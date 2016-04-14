#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

//#define USE_SERIAL Serial
#define PM_SERIAL Serial
//#define SSID ""
//#define PASS ""
String SENSOR_ID = "";
String API_KEY = "";
String API = "http://api.sensorweb.io/sensors/" + SENSOR_ID + "/data";

ESP8266WiFiMulti WiFiMulti;

void setup() {
    PM_SERIAL.begin(9600);
    WiFiManager wifiManager;
    //reset saved settings
//    wifiManager.resetSettings();

    //set custom ip for portal
    //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("SensorWebAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

//    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

//    USE_SERIAL.println();
//    USE_SERIAL.println();
//    USE_SERIAL.println();

//    for(uint8_t t = 4; t > 0; t--) {
//        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
//        USE_SERIAL.flush();
//        delay(1000);
//    }

//    WiFiMulti.addAP(SSID, PASS);

}

int *load() {
  unsigned int pm10 = 0;
  unsigned int pm25 = 0;
  unsigned int pm100 = 0;
  int result[] = {-1, -1, -1};
  int index = 0;
  char value;
  char previousValue;

  while (PM_SERIAL.available()) {
    value = PM_SERIAL.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)){
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm10 = 256 * previousValue + value;
      result[0] = pm10;
    }
    else if (index == 7) {
      pm25 = 256 * previousValue + value;
      result[1] = pm10;
    }
    else if (index == 9) {
      pm100 = 256 * previousValue + value;
      result[2] = pm10;
    } else if (index > 15) {
      break;
    }
    index++;
  }
  while(PM_SERIAL.available()) PM_SERIAL.read();
  return result;
}

void loop() {
    int* pm = load();
    // wait for WiFi connection
    if(pm[1] >= 0 && WiFiMulti.run() == WL_CONNECTED) {

        HTTPClient http;

//        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin(API); //HTTP

//        USE_SERIAL.print("[HTTP] POST...\n");
        // start connection and send HTTP header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpCode = http.POST("pm2_5=" + String(pm[1]) + "&api_key=" + API_KEY);

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
//            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
//                USE_SERIAL.println(payload);
            }
        } else {
//            USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(10000);
}

