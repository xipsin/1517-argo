#include <Arduino.h>

#include "mqtt_handlers.h"

#include "buoyancy.h"

void mqtt_setServo_handler(String msg){
    Serial.print("Received Servo command:");
    Serial.println(msg);
    if (msg == "Up"){
        Serial.println("Floating Up!");
        buoyancyServo_float();
    }
    else if (msg == "Down"){
        Serial.println("Diving Down!");
        buoyancyServo_dive();
    }
    else{
        Serial.println("Wrong servo-command ");
    }
}