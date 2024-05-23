#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define PIN_DATA 17 /*Pin Sensor DS1*/

OneWire oneWire(PIN_DATA);
DallasTemperature sensor = (&oneWire);
float temp_ds1;

void init_sensor() {
    sensor.begin();
    sensor.requestTemperatures();
    temp_ds1 = sensor.getTempCByIndex(0);
    
    if (temp_ds1 == -127) {
        Serial.print("Error connecting to DS1 sensor. Check wiring.");
    }
}

void setup() {
  Serial.begin(115200);
  init_sensor();

}

void loop() {
  Serial.print("Belt temperature: ");
  Serial.println(temp_ds1);
  delay(1000);
}