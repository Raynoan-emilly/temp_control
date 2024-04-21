/***************************************************
Project: Measure the internal temperature of an airfreyer using an infrared temperature sensor MLX90614
Data: 04/17/2024
Authors:
-Flaviano Medeiros da Silva Júnior (flaviano.medeiros@estudante.ufcg.edu.br)
-Raynoan Emilly Silva Batista (raynoan.batista@ee.ufcg.edu.br)
-Jefferson Lopes (jefferson.lopes@ee.ufcg.edu.br)
 ****************************************************/
#include <Arduino.h>
#include <Adafruit_MLX90614.h>

// All the pins can be changed
//#define PIN_SENSOR 5   /*PIN SENSOR*/
#define PIN_UP_BUTTON 5  /*Up temperature*/
#define PIN_DOWN_BUTTON 17 /*Down  temperature*/
#define PIN_START_STOP 16 /*tEMP CONTROL*/
#define PIN_SWITCH 14  /*Swich*/

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
uint16_t target = 50;
const uint16_t limit_max = 100;
const uint16_t limit_min = 40; 
bool mode_state = 0;

static unsigned long last_interrupt_time_up = 0;
static unsigned long last_interrupt_time_down = 0;
static unsigned long last_interrupt_time_ss = 0;

/*------Switch control------*/
void controlSwitch(bool state)
{
  float chosen_temperature;
  digitalWrite(PIN_SWITCH, state);
}

void IRAM_ATTR ISR_UP(){
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time_up > 200) 
  {
    target++;

    if (target >= limit_max){
      target = limit_max;
    }

    Serial.print("UP, target: ");
    Serial.println(target);
  }
  last_interrupt_time_up = interrupt_time;
}
void IRAM_ATTR ISR_DOWN(){
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time_down > 200) 
  {
   target--;

    if (target <= limit_min){
      target = limit_min;
    }
    
    Serial.print("DOWN, target: ");
    Serial.println(target);
  }
  
  last_interrupt_time_down = interrupt_time;

}
void IRAM_ATTR ISR_SS(){
 unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time_ss > 200) 
  {
    mode_state = !mode_state;
   
    if (mode_state) {
      Serial.println("Start");
    }
    else{
      Serial.println("Stop");
    }
  }
  last_interrupt_time_ss = interrupt_time;
}
void setup()
{
  
  Serial.begin(115200);

  pinMode(PIN_SWITCH, OUTPUT);
  pinMode(PIN_UP_BUTTON, INPUT_PULLUP);
  pinMode(PIN_DOWN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_START_STOP,INPUT_PULLUP);
  attachInterrupt(PIN_UP_BUTTON, ISR_UP, FALLING);
  attachInterrupt(PIN_START_STOP, ISR_SS, FALLING);
  attachInterrupt(PIN_DOWN_BUTTON, ISR_DOWN, FALLING);
 
    if (!mlx.begin()) {
      Serial.println("Error connecting to MLX sensor. Check wiring.");
      while (1);
    };

}

void loop()
{
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF());
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");

  Serial.println();
  delay(500);
}