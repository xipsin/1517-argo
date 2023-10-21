#include <Arduino.h>

#include "modules/settings.h"
#include "modules/buoyancy.h"
#include "modules/wifi_mqtt.h"
#include "modules/myEnvSensors.h"

void setup() {
  Serial.begin(SERIAL_SPEED);
  
  setup_wifi();
  mqtt_reconnect();
  
  enders_init();
  envSensors_init();
  buoyancyServo_init();
}

void print_sensorData(String msg){
  Serial.println(msg);
}

bool serialPrintSensorData = true; 

void sensor_update(){
    envSensorData dataStamp = get_envData_stamp();
    envSensorsUpdate(dataStamp);
    String msg = envSensorData_toString(dataStamp);
    if (serialPrintSensorData){
      print_sensorData(msg);
    }
    if (get_mqttMode() == MQTT_IS_ON){
      mqtt_send_sensorData(msg);
    }
}


void loop() {
  if (get_mqttMode() == MQTT_IS_ON)
    mqtt_routine();
  
  static uint32_t envSensorLasttime = millis();
  uint32_t now = millis();

  if (fabs(now - envSensorLasttime) >= ENV_SENSORS_TASK_PERIOD_MS) {
    sensor_update();
    envSensorLasttime = millis();
  }
}
