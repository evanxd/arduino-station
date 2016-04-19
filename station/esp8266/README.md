ESP8266 Arduino Setup Instructions
==================================

## Hardware & Accessories to Prepare in Advance

- ESP8266
    + ESP-01 is more breadboard-friendly, but less pin functions.
    + ESP-12 have more pin functions available, including an ADC, but it's a bit difficult to wiring with breadboard.
    + Both above would work with PM Sensor in this guide, choose according to your specific need.
- PM Sensor
    + Currently only tested with [PMS3003][PMS3003]
- USB 2 TTL Adapter
    + PL2303/CP2102/FT232RL based adapters will work
    + Need to have both 5V and 3.3V output since PM Sensor only accept 5V and ESP8266 requires 3.3V
- Breadboard x 1
- 1k resistors x 3
- Jump wires

## Prepare Software Environment

Install [Arduino IDE][arduino-download] and follow this [installation guide][esp8266-install] to install ESP8266 dependencies in Arduino IDE.

Install [WifiManger][wifi-manager] through Arduino Library Manager.

If your computer can't detect your USB 2 TTL Adapter when plugged in, try to install it's official driver. 
Below are driver download links for supported adapters:
* [PL2303 Driver][PL2303]
* [FT232RL Driver][FT232RL]
* [CP2102 Driver][CP2102]

## Step 1: Wiring ESP8266

USB TTL Adapter Pins| ESP8266 Pins
--- | -----
3V3 | VCC
GND | GND
RXD | TXD
TXD | RXD
3V3 | CH_PD (with resistor)
3V3 | RESET (with resistor)
GND | GPIO0
GND | GPIO15 (only certain model have this)


## Step 2: Configure `API_KEY`, `SENSOR_ID` and Flash Firmware

Open Arduino IDE, paste below code into the IDE:
https://github.com/sensor-web/arduino-station/blob/master/station/esp8266/esp8266-station.ino

Change the `API_KEY` and `SENSOR_ID` to those you get from [SensorWeb dashboard][sensorweb-profile].

Click the **Upload** button and wait until it's done.

## Step 3: Connect Sensor and Get it Running

Take the PM Sensor for example, connect it to your ESP8266 like this:

Unplug TX/RX between USB TTL Adapter and ESP8266,
Plug GPIO0 to 3V3.

USB TTL Adapter Pins| ESP8266 Pins
--- | ----
3V3 | VCC
GND | GND
3V3 | CH_PD (with resistor)
3V3 | RESET (with resistor)
3V3 | GPIO0 (with resistor)
GND | GPIO15 (only certain model have this)

Plug TX/RX between PM Sensor and ESP8266.

PM Sensor Pins| ESP8266 Pins
--- | ----
RXD | TXD
TXD | RXD

Plug 5V/GND from USB TTL Adapter to PM Sensor

USB TTL Adapter Pins| PM Sensor Pins
--- | ----
5V  | VCC
GND | GND

Finally, power on your USB TTL Adapter to make it work.

## Step 4: Setup Wifi

If it didn't connect to the wifi before, it will turn into SoftAP mode automatically. 

Connect to `SensorWebAP` SSID using your PC or smartphone, then a popup will show up for you to enter your wifi credentials. It will reboot after submit.

It should begin transmitting data now!

## Voilà!

Go back to [SensorWeb dashboard][sensorweb-profile] to see your live data.

[PMS3003]: https://goo.gl/CIVGjF
[arduino-download]: https://www.arduino.cc/en/Main/Software
[wifi-manager]: https://github.com/tzapu/WiFiManager#quick-start
[esp8266-install]: http://esp8266.github.io/Arduino/versions/2.1.0/doc/installing.html
[PL2303]: http://goo.gl/KuqY4h
[CP2102]: http://goo.gl/jSnHO0
[FT232RL]: http://goo.gl/IYHhQI
[sensorweb-profile]: http://sensorweb.io/profile
