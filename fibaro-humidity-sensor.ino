#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define uS_TO_S_FACTOR 1000000

RTC_DATA_ATTR int lastValue = 0;
RTC_DATA_ATTR int bootCounter = 0;

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const String FIBARO_IP = "FIBARO_IP_ADDRESS";
const String FIBARO_VARIABLE = "FIBARO_VARIABLE_NAME";
const String FIBARO_AUTH = "FIBARO_AUTH_KEY"; // base64 of your user@name.com:password
const int TIME_TO_SLEEP = 15; // seconds
const int HUMIDITY_THRESHOLD = 5; // %
const int REPORT_VLAUE_AFTER_X_BOOTS = 120;
int wifiConnectionTime = 0;
int currentValue;

Adafruit_BME280 bme; // I2C

void setup() {
//  Serial.begin(115200);

  bme.begin(0x76);
  currentValue = (int)(bme.readHumidity()*100);

//  Serial.print(F("Current value: "));
//  Serial.println(currentValue);
//  Serial.print(F("Last value: "));
//  Serial.println(lastValue);
//  Serial.print(F("Boot counter: "));
//  Serial.println(bootCounter);
//  Serial.print(F("Humidity threshold: "));
//  Serial.println(HUMIDITY_THRESHOLD * 100);
//  Serial.print(F("Humidity difference: "));
//  Serial.println(abs(lastValue - currentValue));

  if ((abs(lastValue - currentValue) > HUMIDITY_THRESHOLD * 100) || (bootCounter >= REPORT_VLAUE_AFTER_X_BOOTS)) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED && wifiConnectionTime <= 5000) {
//      Serial.print(F("WiFi not ready: "));
//      Serial.print(wifiConnectionTime);
//      Serial.println(F("s"));
      wifiConnectionTime += 200;
      delay(200);
    }

    if (WiFi.status()== WL_CONNECTED) {
//      Serial.print(F("WiFi READY!: "));
//      Serial.print(wifiConnectionTime);
//      Serial.println(F("s"));

      HTTPClient http;

      http.begin("http://" + FIBARO_IP + "/api/globalVariables/" + FIBARO_VARIABLE);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("Accept", "application/json");
      http.addHeader("x-fibaro-version", "2");
      http.addHeader("accept-language", "pl");
      http.addHeader("Authorization", "Basic " + FIBARO_AUTH);

      int httpResponseCode = http.PUT("{\"name\": \"" + FIBARO_VARIABLE + "\", \"isEnum\": false, \"readOnly\": false, \"value\": \"" + String(currentValue) + "\"}");
      lastValue = currentValue;
      bootCounter = 0;

      http.end();
    }
  }

  bootCounter++;
//  Serial.println();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {
}
