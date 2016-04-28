ESP8266 Arduino Kit Setup Instructions
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

## Step 2: Flash Firmware

Clone this repo:
```
git clone https://github.com/sensor-web/arduino-station/
```

Open `station/esp8266-kit/esp8266-kit.ino` from Arduino IDE.

Click the **Upload** button to flash firmware and wait until it's done.


## Step 3: Connect Sensor and Get it Running

Take the PM Sensor for example, connect it to your ESP8266 like this:

Unplug TX/RX between USB TTL Adapter and ESP8266,
Plug GPIO0 to 3V3.

Power Source| ESP8266 Pins
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

Plug 5V/GND from USB TTL Adapter or other power source to PM Sensor

Power Source| PM Sensor Pins
--- | ----
5V  | VCC
GND | GND

## Step 4: Setup Wifi and API Key

Power it on.

You'll see a new AP called SensorWeb-XXXXXX (XXXXXX represents a hex string).

Connect to it, a captive portal will pop out.

Enter your wifi credentials, API Key, Sensor ID, and click **Save**.

Wait for it to reboot.

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
