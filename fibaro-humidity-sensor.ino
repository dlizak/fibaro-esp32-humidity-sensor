#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 30

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
String fibaroIpAddress = "FIBARO_IP_ADDRESS";
String fibaroVariableName = "FIBARO_VARIABLE_NAME";
String fibaroBase64Auth = "FIBARO_AUTH_KEY"; // base64 of your user@name.com:password
unsigned int wifiConnectionTime = 0;

Adafruit_BME280 bme; // I2C

void setup() {
  WiFi.begin(ssid, password);
  bme.begin(0x76);

  while (WiFi.status() != WL_CONNECTED && wifiConnectionTime <= 5000) {
    wifiConnectionTime += 200;
    delay(200);
  }
 
  if (WiFi.status()== WL_CONNECTED) {
    HTTPClient http;

    http.begin("http://" + fibaroIpAddress + "/api/globalVariables/" + fibaroVariableName);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Accept", "application/json");
    http.addHeader("x-fibaro-version", "2");
    http.addHeader("accept-language", "pl");
    http.addHeader("Authorization", "Basic " + fibaroBase64Auth);

    int httpResponseCode = http.PUT("{\"name\": \"" + fibaroVariableName + "\", \"isEnum\": false, \"readOnly\": false, \"value\": \"" + String((int)(bme.readHumidity()*100)) + "\"}");
  
    http.end();
  }

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {
}
