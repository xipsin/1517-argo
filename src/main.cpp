#include <Arduino.h>

//#include "modules/ota.h"
#include "modules/settings.h"
#include "modules/buoyancy.h"
#include "modules/wifi_mqtt.h"
#include "modules/myEnvSensors.h"

#define OTA_DISABLED false;
#define OTA_ENABLED true;

bool ota_state = OTA_DISABLED;


void print_sensorData(String msg){
  Serial.println(msg);
}

bool serialPrintSensorData = false; 

void send_msg(String msg){
  if (serialPrintSensorData){
      print_sensorData(msg);
    }
    if (get_mqttMode() == MQTT_IS_ON){
      mqtt_send_sensorData(msg);
    }
}

void sensor_update(){
    envSensorData dataStamp = get_envData_stamp();
    envSensorsUpdate(dataStamp);
    String msg = dataframe_csvData_toString(dataStamp);
    send_msg(msg);

    #ifdef ___DEBUG___
      msg = envSensorData_toString(dataStamp);
      DPRINTLN(msg);
    #endif
}


void setup() {
  Serial.begin(SERIAL_SPEED);
  
  setup_wifi();
  //ota_init();
  mqtt_reconnect();
  
  enders_init();
  envSensors_init();
  buoyancyServo_init();

  send_msg(dataframe_csvHeader_toString());
}



void loop() {
  if (get_mqttMode() == MQTT_IS_ON){
    mqtt_routine();
  }
  
  /*
  if (ota_state){
    ota_tick();
  }
  */
  

  static uint32_t envSensorLasttime = millis();
  uint32_t now = millis();

  if (fabs(now - envSensorLasttime) >= ENV_SENSORS_TASK_PERIOD_MS) {
    sensor_update();
    envSensorLasttime = millis();
  }
}
