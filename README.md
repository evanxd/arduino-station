Arduino Station
===============
Sensor station built with Arduino-like boards.

## Supported Boards Setup

There are various ways to enable internet connection on Arduino-like boards, either adding ethernet/wifi/GSM shields/modules or using ethernet/wifi/2G/3G integrated boards. Initially we are trying to focus on 3 less difficult and more affordable boards setup.

1. Particle Photon
  - An Arduino-like (STM32) board with wifi connectivity
  - Easy to use devtools, can do OTA on the web
  - Porting from Arduino code is easy
  - Good community libraries
  - 1MB flash
  - $19 USD
  - Wiring difficulty: normal
  - [Setup instructions >>][particle-setup]
2. ESP8266 Arduino
  - Compact wifi module with Arduino-like capabilities
  - Integrated well with Arduino IDE
  - Useful libraries like http server/client, wifi manager, OTA...
  - 1MB ~ 4MB flash depends on the version
  - $5 USD
  - Wiring difficulty: a little complex
  - [Setup instructions >>][esp-setup]
3. Arduino + ESP8266 Serial Wifi module 
  - Use AT commands in Arduino to control wifi module via serial pins
  - 32KB flash
  - $27 USD (clones could be much cheaper)
  - Wiring difficulty: a little complex
  - [Setup instructions >>][esp-serial-setup]

## Supported Sensors

Since our first initiative is to build a PM2.5 sensor network, we only have one PM sensor support for now.

- Plantower PM Sensor
  - Currently only tested with [PMS3003][PMS3003]
  - 5V input, communicate via UART
  - Great accuracy according to [comparative measurment on AQICN][aqicn-measurment]
  - $17 USD

## Maintainers
* [Evan Tseng](https://github.com/evanxd)
* [Eddie Lin](https://github.com/yshlin)

[PMS3003]: https://goo.gl/CIVGjF
[aqicn-measurment]: http://aqicn.org/sensor/
[photon-setup]: https://github.com/sensor-web/arduino-station/tree/master/station/photon
[esp-setup]: https://github.com/sensor-web/arduino-station/tree/master/station/esp8266
[esp-serial-setup]: https://github.com/sensor-web/arduino-station/tree/master/station/esp8266-serial2wifi