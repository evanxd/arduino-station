Arduino + ESP8266 Serial Wifi Module Setup Instructions
=======================================================

## Hardware & Accessories to Prepare in Advance

- ESP8266
    + ESP-01 with default firmware would be sufficient since we only need UART pins to send AT commands, and it's more breadboard-friendly, too.
- PM Sensor
    + Currently only tested with [PMS3003][PMS3003]
- Breadboard x 1
- 1k resistors x 2
- Jump wires

## Prepare Software Environment

Install [Arduino IDE][arduino-download] if you don't have it on your PC.

## Step 1: Connect Sensors

Take the PM Sensor for example, connect it to your Arduino like this:

PM Sensor Pins| Arduino Pins
---------- | ----------
VCC (PIN1) | 5V
GND (PIN2) | GND
RXD (PIN4) | D11 (SoftSerial TX)
TXD (PIN5) | D10 (SoftSerial RX)

* Since TX/RX would be occupied by ESP8266 wifi serial module, let's use Software Serial pins to access the PM Sensor.
* We can't flash Arduino when TX/RX pin occupied, so we need to connect ESP8266 module in [step 3][step-3] after firmware flashed.

## Step 2: Configure `API_KEY` and `SENSOR_ID`, Wifi Credentials and Flash Firmware

Open Arduino IDE, paste below code into the IDE:
https://github.com/sensor-web/arduino-station/blob/master/station/esp8266-serial2wifi/esp8266-serial2wifi-station.ino

Change the `API_KEY` and `SENSOR_ID` to those you get from [SensorWeb dashboard][sensorweb-profile].

Change the `SSID` and `PASS` with your wifi credentials.

Click the **Upload** button and wait until it's done.

## Step 3: Connect ESP8266 Serial Wifi Module

Connect the ESP8266 serial wifi module to Arduino:

Arduino Pins| ESP8266 Pins
---- | -----
3.3V | VCC
GND  | GND
RXD  | TXD
TXD  | RXD
3.3V | CH_PD (with resistor)
3.3V | RESET (with resistor)
GND  | GPIO0
GND  | GPIO15 (only certain model have this)

And power on Arduino to get it up and running.

## Voilà!

Go back to [SensorWeb dashboard][sensorweb-profile] to see your live data.

[arduino-download]: https://www.arduino.cc/en/Main/Software
[step-3]: #step-3-connect-esp8266-serial-wifi-module
[PMS3003]: https://goo.gl/CIVGjF
[sensorweb-profile]: http://sensorweb.io/profile
