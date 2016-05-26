#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager

#define DATA_RESET 0
#define API_DEBUG 0
#define PM_SERIAL Serial

//define your default values here, if there are different values in config.json, they are overwritten.
char api_server[40] = "api.sensorweb.io";
char api_port[6] = "80";
char api_key[34] = "";
char sensor_id[34] = "";
String api = "http://";

int pm1 = -1;
int pm2_5 = -1;
int pm10 = -1;

unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change :
const long interval = 10000;
#define LENG 32
char buf[LENG];

ESP8266WiFiMulti WiFiMulti;

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
//  Serial.println("Should save config");
  shouldSaveConfig = true;
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  PM_SERIAL.begin(9600);
  // put your setup code here, to run once:

  //clean FS, for testing
//  if (DATA_RESET) {
//    SPIFFS.format();
//  }

  if (SPIFFS.begin()) {
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        if (json.success()) {
//          if (API_DEBUG) {
            strcpy(api_server, json["api_server"]);
            strcpy(api_port, json["api_port"]);
//          }
          strcpy(api_key, json["api_key"]);
          strcpy(sensor_id, json["sensor_id"]);

        }
      }
    }
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_api_server("api_server", "API Server", api_server, 40);
  WiFiManagerParameter custom_api_port("api_port", "API Port", api_port, 5);
  WiFiManagerParameter custom_api_key("api_key", "API Key", api_key, 32);
  WiFiManagerParameter custom_sensor_id("sensor_id", "Sensor ID", sensor_id, 32);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //wifiManager.setDebugOutput(false);
  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
//  wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  WiFiManagerParameter label_api_cfg("<br><br><label>API Config:</label>");
  wifiManager.addParameter(&label_api_cfg);
  wifiManager.addParameter(&custom_sensor_id);
  wifiManager.addParameter(&custom_api_key);

//  if (API_DEBUG) {
    WiFiManagerParameter label_server_cfg("<br><br><label>Server Config:</label>");
    wifiManager.addParameter(&label_server_cfg);
    wifiManager.addParameter(&custom_api_server);
    wifiManager.addParameter(&custom_api_port);
//  }
  
  if (DATA_RESET) {
    //reset settings - for testing
    wifiManager.resetSettings();
  }

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  String chipid = String(ESP.getChipId(), HEX);
  chipid.toUpperCase();
  String ssid = "SensorWeb-"+chipid;
  if (!wifiManager.autoConnect(ssid.c_str(), "sensorweb.io")) {
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi

  //read updated parameters
//  if (API_DEBUG) {
    strcpy(api_server, custom_api_server.getValue());
    strcpy(api_port, custom_api_port.getValue());
//  }
  strcpy(api_key, custom_api_key.getValue());
  strcpy(sensor_id, custom_sensor_id.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["api_server"] = api_server;
    json["api_port"] = api_port;
    json["api_key"] = api_key;
    json["sensor_id"] = sensor_id;

    File configFile = SPIFFS.open("/config.json", "w");

    json.printTo(configFile);
    configFile.close();
    //end save

  }

  //compose API url
  api += String(api_server) + ":" + String(api_port) + "/sensors/" + String(sensor_id) + "/data";

}


void load() {
  if (PM_SERIAL.available()) {
    PM_SERIAL.readBytes(buf, LENG);
    pm1 = buf[10]*0x100 + buf[11];
    pm2_5 = buf[12]*0x100 + buf[13];
    pm10 = buf[14]*0x100 + buf[15];
  }
}

void loop() {
    
    load();
    
    unsigned long currentMillis = millis();
  
    if (currentMillis - previousMillis < interval) {
      return;
    }
    previousMillis = currentMillis;    // wait for WiFi connection

    digitalWrite(LED_BUILTIN, HIGH);
    // wait for WiFi connection
    if(pm2_5 >= 0 && WiFiMulti.run() == WL_CONNECTED) {

        HTTPClient http;

        // configure traged server and url
        http.begin(api); //HTTP

        // start connection and send HTTP header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpCode = http.POST("pm25=" + String(pm2_5) + "&apiKey=" + String(api_key));

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
            }
        }

        http.end();
    }
    digitalWrite(LED_BUILTIN, LOW);
}

