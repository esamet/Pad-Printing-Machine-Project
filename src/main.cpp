#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <header_file.hpp>

const int sensor_pin = 27; // Dikey Sensör pin numarası
const int pedal_pin = 26;
const int dikeymotor_pin = 5;   // Motor pin numarası
const int yataymotor_pin = 18;   // Motor 2 pin numarası
const int otomatik_pin = 25; 
const int counter_reset_pin = 33;
const int shake_mode_pin = 2;
volatile int state = 0;
volatile bool action_done = false;
volatile unsigned long prev_time = 0;
unsigned long time_treshold = 50;
volatile bool otomatik = false;
bool pedal_state;
bool stopped;
bool pedal_pos = true;
int counter = 0;
bool shake_mod;
void sensor_fonksiyon() {
  unsigned long current_time = millis();
  if (current_time - prev_time > time_treshold) {

    if(!otomatik){
      if (!pedal_pos) {
        state++;
        lcd.clear();
      }
    }
    else if(otomatik){
      if(state==5)
        state=0;
      else
        if (!pedal_pos && !stopped) {
          state++;
          lcd.clear();
        }
    }
    action_done = false;

  }
    prev_time = current_time;

}

void aksiyon() {
  if(!otomatik){
  if (!action_done && !pedal_pos) {
    switch (state) {
        case 0:
          digitalWrite(dikeymotor_pin, HIGH);
          digitalWrite(yataymotor_pin, HIGH);
          break;
        case 1:
          digitalWrite(dikeymotor_pin, LOW);
          digitalWrite(yataymotor_pin, HIGH);
          break;
        case 2:
          digitalWrite(dikeymotor_pin, HIGH);
          digitalWrite(yataymotor_pin, HIGH);
          break;
        case 3:
          digitalWrite(dikeymotor_pin, HIGH);
          digitalWrite(yataymotor_pin, LOW);
          break;
        case 4:
          digitalWrite(yataymotor_pin, LOW);
          digitalWrite(dikeymotor_pin, LOW);
          break;
        case 5:
          digitalWrite(dikeymotor_pin, LOW);
          digitalWrite(dikeymotor_pin, HIGH);
        pedal_pos = true;
        state = 0;
        counter++;
        break;
      default:
        break;
    }
    action_done = true;
    if (state == 5)
      action_done = false;
  }}
  else if(otomatik){
     if(!action_done  && !pedal_pos){
      switch(state)
      {
        case 0:
          digitalWrite(dikeymotor_pin, HIGH);
          digitalWrite(yataymotor_pin, HIGH);
          break;
        case 1:
          digitalWrite(dikeymotor_pin, LOW);
          digitalWrite(yataymotor_pin, HIGH);
          break;
        case 2:
          digitalWrite(dikeymotor_pin, HIGH);
          digitalWrite(yataymotor_pin, HIGH);
          break;
        case 3:
          digitalWrite(dikeymotor_pin, HIGH);
          digitalWrite(yataymotor_pin, LOW);
          break;
        case 4:
          digitalWrite(yataymotor_pin, LOW);
          digitalWrite(dikeymotor_pin, LOW);
          break;
        case 5:
          digitalWrite(dikeymotor_pin, LOW);
          digitalWrite(dikeymotor_pin, HIGH);
          counter++;
          break;

        default:
          break;
      
      }
    action_done = true;
    }
  }

}

void setup() {
  pinMode(sensor_pin, INPUT_PULLDOWN);
  pinMode(pedal_pin, INPUT_PULLDOWN);
  pinMode(otomatik_pin, INPUT_PULLDOWN);
  pinMode(counter_reset_pin, INPUT_PULLDOWN);
  pinMode(shake_mode_pin, INPUT_PULLDOWN);
  pinMode(dikeymotor_pin, OUTPUT);
  pinMode(yataymotor_pin, OUTPUT);
  Serial.begin(115200);
  //attachInterrupt(digitalPinToInterrupt(sensor_pin), sensor_fonksiyon, RISING);
  digitalWrite(yataymotor_pin, LOW);
  digitalWrite(dikeymotor_pin, HIGH);
  lcd.init();
  lcd.backlight();
  lcd.clear();  // Clear the LCD screen
}

bool sensor_state;
bool prev_sensor_state= 1;
bool pedal_prev_state;
bool prev_pedal_pos = 1;
bool prev_stopped ;

void loop() {

  if(state==0) otomatik = digitalRead(otomatik_pin);
  aksiyon();
  sensor_state = !digitalRead(sensor_pin);
  pedal_state = digitalRead(pedal_pin);

  if (pedal_state > pedal_prev_state) {
    pedal_pos = !pedal_pos;
    if (pedal_pos ) {
      action_done = false; // Pedal pozisyonu sıfır olduğunda action_done'u sıfırla
    }
    
  } 
  if (sensor_state > prev_sensor_state) {
    sensor_fonksiyon();
  }
if(prev_pedal_pos<pedal_pos && pedal_state > pedal_prev_state) stopped = true;
else if(prev_pedal_pos > pedal_pos && pedal_state > pedal_prev_state) stopped = false;
if(prev_stopped>stopped) {
  state++;
  lcd.clear();
  }
  Serial.printf("state: %d Prev_time: %lu , pedal_pos: %d , action_done: %d , counter: %d stopped: %d shake_mod: %d \n",
  state, prev_time, pedal_pos, action_done,counter, stopped,digitalRead(shake_mode_pin));

  prev_sensor_state = sensor_state;
  pedal_prev_state = pedal_state;
  prev_pedal_pos = pedal_pos;
  prev_stopped = stopped;
  show_display(state,otomatik,counter);
          
  if(digitalRead(counter_reset_pin) == HIGH){
    lcd.clear(); 
    counter = 0;
  } 
while(digitalRead(shake_mode_pin) && state == 0) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mod: Calkalama");
  digitalWrite(dikeymotor_pin,HIGH);
  digitalWrite(yataymotor_pin,HIGH);
  delay(2000);
  digitalWrite(yataymotor_pin,LOW);
  delay(2000);
  lcd.clear();

}
}
