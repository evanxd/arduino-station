#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* SSID = "ssid";
const char* PASSWORD = "password";
const String SERVER_ADDRESS = "api.sensorweb.io";
const String SERVER_PORT = "80";
const String SENSOR_ID = "sensorId";
const String API_KEY = "apiKey";
const String REST_API = "http://" + SERVER_ADDRESS + ":" + SERVER_PORT + "/sensors/" + SENSOR_ID + "/data";

unsigned int pm10 = 0;
unsigned int pm25 = 0;
unsigned int pm100 = 0;
HTTPClient http;

void setup() {
  Serial.begin(9600);

  Serial.println("Mac Address: " + WiFi.macAddress());

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}

void loop() {
  int index = 0;
  char value;
  char previousValue;

  while (Serial.available()) {
    value = Serial.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)){
      Serial.println("Cannot find the data header.");
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm10 = 256 * previousValue + value;
      Serial.print("{ ");
      Serial.print("\"pm10\": ");
      Serial.print(pm10);
      Serial.print(", ");
    }
    else if (index == 7) {
      pm25 = 256 * previousValue + value;
      Serial.print("\"pm25\": ");
      Serial.print(pm25);
      Serial.print(", ");
    }
    else if (index == 9) {
      pm100 = 256 * previousValue + value;
      Serial.print("\"pm100\": ");
      Serial.print(pm100);
    } else if (index > 15) {
      break;
    }
    index++;
  }
  while(Serial.available()) Serial.read();
  Serial.println(" }");

  http.begin(REST_API);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST("pm25=" + String(pm25) + "&apiKey=" + API_KEY);
  http.writeToStream(&Serial);
  http.end();

  delay(60000);
}

