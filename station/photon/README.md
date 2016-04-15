Photon Setup Instructions
=========================

## Step 1: Setup Your Photon

Follow the [quickstart instruction][photon-setup] on photon website to create an particle account, connect your photon to the internet, and claim it under your account.

## Step 2: Connect Sensors

Take the PM Sensor for example, connect it to your photon like this:

PM Sensor Pins| Photon Pins
---------- | ----------
VCC (PIN1) | VIN (5V+ when powered via USB) 
GND (PIN2) | GND
RXD (PIN4) | TXD
TXD (PIN5) | RXD

## Step 3: Configure `API_KEY` and `SENSOR_ID`

Go to https://build.particle.io, and create an new app `SENSORWEB-STATION`.

Click on **Libraries** button, and find `HTTPCLIENT` in **Community Libraries**

Get in `HTTPCLIENT` library detail, and click on **INCLUDE IN APP**.

Go back to the app you just created, and paste following code inside the ino file.

Change the `SENSOR_ID` and `API_KEY` to those you get from [SensorWeb dashboard][sensorweb-profile].

## Step 4: Flash Firmware and Get it Running

Make sure your photon is connected and breathing cyan.

Click on **Devices** button in https://build.particle.io, and start the device you want to flash.

Click **Flash** button, if it's blinking purple, and reboot to breathing cyan after a while, it should be working properly.

Go back to [SensorWeb dashboard][sensorweb-profile] to see your live data.

[photon-setup]: https://docs.particle.io/guide/getting-started/start/photon/#step-1-power-on-your-device
[sensorweb-profile]: http://sensorweb.io/profile