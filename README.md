# fibaro-humidity-sensor

Humidity sensor for Fibaro HC3 (probably will work with HC2 also).

## Hardware used in project:
- Wemos LOLIN32 with 3.7V battery connector
- BME280 sensor
- 18650 Li-ion battery

## Installation guide:
1. Go to Fibaro HC3 variables settings and create standard variable. (Don't know it's necessary)
2. Connect BME280 using I2C and connect the battery.
3. Change variable values in sketch using your WiFi SSID and password, IP address of your Fibaro HC3 (make sure it's static IP), variable name, and Base64 value of your username and password.

Currently supports only HTTP connection.
By default data is updated every 30s. Between this time, ESP32 is in deep sleep mode.
