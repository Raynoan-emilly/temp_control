/***************************************************
Project: Measure the internal temperature of an airfreyer using an infrared temperature sensor MLX90614
Data: 04/17/2024
Authors:
-Flaviano Medeiros da Silva Júnior
-Raynoan Emilly Silva Batista (raynoan.batista@ee.ufcg.edu.br)
-
 ****************************************************/
#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_MLX90614.h>
//All the pins can be changed
#define PIN_SENSOR 5     /*PIN SENSOR*/
#define UP_BUTTON 14     /*Up temperature*/
#define DOWN_BUTTON 19   /*Down  temperature*/
#define PIN_SWICH 32     /*Swich*/



Adafruit_MLX90614 mlx = Adafruit_MLX90614();

  /*----Start sensor!----*/
  void init_sensor(){
    mlx.begin;

    while (!Serial);
    if (!mlx.begin()) {
      Serial.println("Error connecting to MLX sensor. Check wiring.");
      while (1);
      };
  }

    /*------Switch control------*/
  void controlSwitch(bool state ){
    float chosen_temperature;
    ........................
    digitalWrite(PIN_SWICH, state);
  }

void setup() {

  Serial.begin(115200);

      pinMode(PIN_SWICH, OUTPUT);
      pinMode(UP_BUTTON, INPUT_PULLUP);
      pinMode(DOWN_BUTTON, INPUT_PULLUP);

    /*------Read Ambient Temperature------*/
    float getTemperature(){
     float TemperatureAmbient = float(mlx.readAmbientTempC());
      return (TemperatureAmbient);
   }

}

void loop() {
  init_sensor();
  float temperature = getTemperature();
  int choice;


  
  delay(500);
}