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
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <Wire.h>

// All the pins can be changed
#define DEBOUNCE_DELAY 250
#define PIN_UP_BUTTON 19   /*Up temperature*/
#define PIN_DOWN_BUTTON 18 /*Down  temperature*/
#define PIN_START_STOP 5   /*tEMP CONTROL*/
#define PIN_RELE_RES 4    /*Swich*/
#define PIN_RELE_PWR 16
#define LINHAS 4
#define COLUNAS 20
#define ADRESS 0X27
#define PIN_DATA 17 /*Pin Sensor DS1*/

LiquidCrystal_I2C lcd(ADRESS, COLUNAS, LINHAS);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
OneWire oneWire(PIN_DATA);
DallasTemperature ds1 = (&oneWire);

const uint16_t limit_max_amb = 110;
const uint16_t limit_max = 100;
const uint16_t limit_min = 20;

uint16_t target = 40;
uint8_t precision = 1;

float temp_ds1;
float temp_mlx_obj;
float temp_mlx_amb;

/**
 * @brief debounce algorithm for the buttons
 * 
 */
unsigned long last_interrupt_time_up = 0;
unsigned long last_interrupt_time_down = 0;
unsigned long last_interrupt_time_ss = 0;

/**
 * @brief true means that is allowed to turn on the resistance
 * 
 */
bool flag_power = false;
bool flag_protection = false;
bool flag_control = false;

/*------Switch control------*/
void control_loop() {
    temp_mlx_obj = mlx.readObjectTempC();

    if (temp_mlx_obj >= target + precision) {
        flag_control = false;
    }
    else if (temp_mlx_obj <= target - precision) {
        flag_control = true;
    }
}

void protection_mode() {
    ds1.requestTemperatures();
    temp_ds1 = ds1.getTempCByIndex(0);
    
    if (temp_ds1 == -127) {
        Serial.print("Error connecting to DS1 sensor. Check wiring.");
        
        flag_protection = false;
    }
    else {
        if (temp_ds1 >= limit_max_amb) {
            flag_protection = false;
        }
        else {
            flag_protection = true;
        }
    }
}

void IRAM_ATTR ISR_UP() {
    unsigned long interrupt_time = millis();

    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time_up > DEBOUNCE_DELAY) {
        target++;

        if (target >= limit_max) {
            target = limit_max;
        }

        Serial.print("UP, target: ");
        Serial.println(target);
    }

    last_interrupt_time_up = interrupt_time;
}

void IRAM_ATTR ISR_DOWN() {
    unsigned long interrupt_time = millis();

    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time_down > DEBOUNCE_DELAY) {
        target--;

        if (target <= limit_min) {
            target = limit_min;
        }

        Serial.print("DOWN, target: ");
        Serial.println(target);
    }

    last_interrupt_time_down = interrupt_time;
}

void IRAM_ATTR ISR_SS() {
    unsigned long interrupt_time = millis();

    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time_ss > DEBOUNCE_DELAY) {
        flag_power = !flag_power;

        if (flag_power) {
            Serial.println("Start");
        } else {
            Serial.println("Stop");
        }
    }

    last_interrupt_time_ss = interrupt_time;
}

void show_temp() {
    lcd.setCursor(13, 0);
    lcd.print(temp_mlx_obj);

    lcd.setCursor(8, 1);
    lcd.print(target);

    Serial.print("obejct temp: ");
    Serial.println(temp_mlx_obj);

    lcd.setCursor(0, 2);
    if (flag_power && flag_protection && flag_control) {
        lcd.print("Heating!");
    }
    else {
        lcd.print("        ");
    }

    lcd.setCursor(13, 1);
    lcd.print(temp_ds1);

    lcd.setCursor(0, 3);
    if (flag_power) {
        lcd.print("Powered ON ");
    }
    else {
        lcd.print("Powered OFF");
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(PIN_RELE_RES, OUTPUT);
    pinMode(PIN_RELE_PWR, OUTPUT);
    pinMode(PIN_UP_BUTTON, INPUT_PULLUP);
    pinMode(PIN_DOWN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_START_STOP, INPUT_PULLUP);

    digitalWrite(PIN_RELE_PWR, HIGH);
    digitalWrite(PIN_RELE_RES, HIGH);

    attachInterrupt(PIN_UP_BUTTON, ISR_UP, FALLING);
    attachInterrupt(PIN_START_STOP, ISR_SS, FALLING);
    attachInterrupt(PIN_DOWN_BUTTON, ISR_DOWN, FALLING);

    /* init display LCD */
    lcd.init();
    lcd.backlight();
    lcd.clear();

    /* init MLX sensor and check if it's connected */
    if (!mlx.begin()) {
        Serial.println("Error connecting to MLX sensor. Check wiring.");
        
        lcd.setCursor(0, 0);
        lcd.print("ERROR: 0x01");

        /* lock processor */
        while (true) {
            ;
        }
    }

    /* init DS18B20 sensor and check if it's working */
    ds1.begin();
    ds1.requestTemperatures();
    temp_ds1 = ds1.getTempCByIndex(0);
    if (temp_ds1 == -127) {
        Serial.println("Error connecting to DS1 sensor. Check wiring.");
        
        lcd.setCursor(0, 0);
        lcd.print("ERROR: 0x02");

        /* lock processor */
        while (true) {
            ;
        }
    }

    lcd.setCursor(0, 0);
    lcd.print("Temp object: ");
    
    lcd.setCursor(0, 1);
    lcd.print("Target: ");
}

void loop() {
    protection_mode();
    control_loop();

    show_temp();

    /* correct for low level active of the relay */
    digitalWrite(PIN_RELE_RES, !(flag_power && flag_protection && flag_control));
    digitalWrite(PIN_RELE_PWR, !flag_power);

    Serial.print("f_pwr: ");
    Serial.print(flag_power);
    Serial.print(" , f_pro: ");
    Serial.print(flag_protection);
    Serial.print(", f_con: ");
    Serial.println(flag_control);

    delay(250);
}