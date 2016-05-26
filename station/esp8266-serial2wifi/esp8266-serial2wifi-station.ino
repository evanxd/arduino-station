#include <SoftwareSerial.h>
#define SSID ""
#define PASS ""
#define SENSOR_ID ""
#define API_KEY ""
#define HOST "api.sensorweb.io"
#define PORT "80"
#define REQ(x) "POST /sensors/"x"/data HTTP/1.0\r\nContent-Length: "
#define REQ2 "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n"
//#define REQ = "GET / HTTP/1.0\r\n\r\n"
//SoftwareSerial dbgSerial(10, 11); // RX, TX
SoftwareSerial PM_SERIAL(10, 11); // RX, TX

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.setTimeout(5000);
  PM_SERIAL.begin(9600);
  // dbgSerial.begin(9600); //can't be faster than 19200 for softserial
  // dbgSerial.println("ESP8266 Demo");
  //test if the module is ready
  Serial.println("AT+RST");
  delay(1000);
  if(Serial.find("ready"))
  {
    //   dbgSerial.println("Module is ready");
  }
  else
  {
    //   dbgSerial.println("Module have no response.");
    while(1);
  }
  delay(1000);
  //connect to the wifi
  boolean connected=false;
  for(int i=0;i<5;i++)
  {
    if(connectWiFi())
    {
      connected = true;
      break;
    }
  }
  if (!connected){while(1);}
  delay(5000);
  //print the ip addr
  /*Serial.println("AT+CIFSR");
    dbgSerial.println("ip address:");
    while (Serial.available())
    dbgSerial.write(Serial.read());*/
  //set the single connection mode
  Serial.println("AT+CIPMUX=0");
}
void loop()
{
  int *pm = load();
  if (pm[1] < 0) {
    return;
  }
  String body = "pm25=" + String(pm[1]) + "&apiKey=" + API_KEY;
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += HOST;
  cmd += "\",";
  cmd += PORT;
  Serial.println(cmd);
  // dbgSerial.println(cmd);
  if(Serial.find("Error")) return;
  cmd = REQ(SENSOR_ID);
  cmd += body.length();
  cmd += REQ2;
  cmd += body;
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">"))
  {
    //   dbgSerial.print(">");
  }else
  {
    Serial.println("AT+CIPCLOSE");
    //     dbgSerial.println("connect timeout");
    delay(1000);
    return;
  }
  Serial.print(cmd);
  delay(2000);
  //Serial.find("+IPD");
  while (Serial.available())
  {
    char c = Serial.read();
    //     dbgSerial.write(c);
    //     if(c=='\r') dbgSerial.print('\n');
  }
  //   dbgSerial.println("====");
  delay(10000);
}
boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  //   dbgSerial.println(cmd);
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("OK"))
  {
    //     dbgSerial.println("OK, Connected to WiFi.");
    return true;
  }else
  {
    //       dbgSerial.println("Can not connect to the WiFi.");
    return false;
  }
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