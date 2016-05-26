#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define PM_SERIAL Serial
#define SSID ""
#define PASS ""
String SENSOR_ID = "";
String API_KEY = "";
String API = "http://api.sensorweb.io/sensors/" + SENSOR_ID + "/data";

int pm1 = -1;
int pm2_5 = -1;
int pm10 = -1;

unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change :
const long interval = 10000;
#define LENG 32
char buf[LENG];

ESP8266WiFiMulti WiFiMulti;

void setup() {
    PM_SERIAL.begin(9600);


    for(uint8_t t = 4; t > 0; t--) {
        delay(1000);
    }

    WiFiMulti.addAP(SSID, PASS);

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
    // wait for WiFi connection
    if(pm2_5 >= 0 && WiFiMulti.run() == WL_CONNECTED) {

        HTTPClient http;

        // configure traged server and url
        http.begin(API); //HTTP

        // start connection and send HTTP header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpCode = http.POST("pm25=" + String(pm2_5) + "&apiKey=" + API_KEY);

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
}

