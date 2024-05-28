#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SPI.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

double temp_obj;

void setup() {
  Serial.begin(9600);
  mlx.begin();

  if (!mlx.begin()) {
        Serial.println("Error connecting to MLX sensor. Check wiring.");

        /* lock processor */
        while (true) {
            ;
        }
    }

}

void loop() {
  temp_obj = mlx.readObjectTempC();
  Serial.print("Belt temperature: ");
  Serial.println(temp_obj);
  
  delay(1000);
}