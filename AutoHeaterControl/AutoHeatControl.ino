#include <Timer.h>
#include <microDS18B20.h>
MicroDS18B20<2> sensor;
Timer timer;
#define Fan 5  // Вентилятор
#define quartzPin 6  // Кварцевая лампа
#define heater_1 7  // Нагреватель №1
#define heater_2 8  // Нагреватель №2
int Temp;
int flag = 1;
unsigned long cur_ms = 0;
unsigned long timer1 = 0;
unsigned long timer2 = 0;
bool relayOn = false;     // Состояние реле
bool startTimer = false;  // Флаг запуска таймера
bool quartzOn = true;
bool stateHeater1 = false;
bool stateHeater2 = false;

void setup() {
  pinMode(Fan, OUTPUT);
  pinMode(quartzPin, OUTPUT);
  pinMode(heater_1, OUTPUT);
  pinMode(heater_2, OUTPUT);
  digitalWrite(Fan, HIGH);
  digitalWrite(quartzPin, HIGH);
  digitalWrite(heater_1, LOW);
  digitalWrite(heater_2, LOW);
  Serial.begin(9600);
  timer.every(20000, quartz);
  timer.every(1000, debug);
}
void debug(){
  Serial.println(stateHeater1);
  Serial.print("temp: ");
  Serial.println(Temp);
  Serial.print("Quartz: ");
  Serial.println(quartzOn);
}
void quartz (){
  if (quartzOn == false){
    quartzOn = true;
    // Serial.print("Quartz:true ");
    // Serial.println(quartzOn);
    digitalWrite(quartzPin, HIGH);
  } else if (quartzOn == true){
    quartzOn = false;
    // Serial.print("Quartz: ");
    // Serial.println(quartzOn);
    digitalWrite(quartzPin, LOW);
  }
}
void loop() {
  timer.update();
  Temp = sensor.getTempInt();

  if (millis() - timer1 >= 5000 && Temp <= 60 && stateHeater1 == false){ // Устанавливаем время через которое запустится первый нагреватель и температуру
    stateHeater1 = true;
    digitalWrite(heater_1, HIGH);
    Serial.println("Heater 1 turn on");
  } else if (millis() - timer2 >= 10000 && Temp <=40 && stateHeater2 == false) {
    stateHeater2 = true;
    digitalWrite(heater_2, HIGH); 
    Serial.println("Heater 2 turn on");
  } else if (Temp >= 50 && stateHeater2 == true){
    stateHeater2 = false;
    digitalWrite(heater_2, LOW);
    Serial.println("Heater 2 turn off");
  } else if (Temp >= 60){
    stateHeater1 = false;
    stateHeater2 = false;
    digitalWrite(heater_1, LOW);
    digitalWrite(heater_2, LOW);
    Serial.println("Attention overheating");
    Serial.println("Heater 1 and 2 turn off");
  }
}
