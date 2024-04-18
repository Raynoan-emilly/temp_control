/***************************************************
Project: Measure the internal temperature of an airfreyer using an infrared temperature sensor MLX90614
Data: 04/17/2024
Authors:
-Flaviano Medeiros da Silva Júnior
-
-
 ****************************************************/

#include <Adafruit_MLX90614.h>


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);

  /*----Start sensor!----*/
  void init_sensor(){
    mlx.begin;

    while (!Serial);
    if (!mlx.begin()) {
      Serial.println("Error connecting to MLX sensor. Check wiring.");
      while (1);
      };
  }

  /*------Read Ambient Temperature------*/
  float getTemperature(){
    float TemperatureAmbient = float(mlx.readAmbientTempC());
    return (TemperatureAmbient);
  }

  /*------Switch control------*/
  void controlSwitch(){
    float chosen_temperature;
    ........................
  }
}

void loop() {
  init_sensor();
  float temperature = getTemperature();
  int choice;


  
  delay(500);
}