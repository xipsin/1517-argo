#include <ESP32Servo.h>

#include "buoyancy.h"


#define CON_UP_PIN 23  // точно вверх
#define CON_DOWN_PIN 36  // точно вниз
#define BUOYANCY_SERVO_PIN 18

#define DOWN 180
#define STOP 90
#define UP 0

Servo buoyancyServo;

void enders_init(){
  pinMode(CON_UP_PIN, INPUT_PULLUP);
  pinMode(CON_DOWN_PIN, INPUT_PULLUP);
}

bool check_conUpEnder(){
 return digitalRead(CON_UP_PIN);
  Serial.print("                              CON_UP_PIN: ");
 Serial.println(digitalRead(CON_UP_PIN));
}

bool check_conDownEnder(){
 return digitalRead(CON_DOWN_PIN);
 Serial.print("                                CON_DOWN_PIN: ");
 Serial.println(digitalRead(CON_DOWN_PIN));
}

void buoyancyServo_init(){
 buoyancyServo.attach(BUOYANCY_SERVO_PIN, 544, 2400);
}

void buoyancyServo_dive(){
 buoyancyServo.write(UP);
 while (!check_conUpEnder())
 {
 delay(2000);
 buoyancyServo.write(STOP);
 }
}

void buoyancyServo_float(){
 buoyancyServo.write(DOWN);
 while (!check_conDownEnder())
 {delay(2000);
buoyancyServo.write(STOP);}
}
