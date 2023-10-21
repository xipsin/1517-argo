#ifndef ___SETTINGS_H___
#define ___SETTINGS_H___


#define ___DEBUG___
//#define ___OTA___
#define ___LAUNCH_WITHOUT_MS5837___


#define SERIAL_SPEED 115200


///////////////////////////////////////////////////////
////////////////////  TIMINGS /////////////////////////
///////////////////////////////////////////////////////

#define ENV_SENSORS_TASK_PERIOD_MS 1000
#define MS5837_ERR_INIT_TIMEOUT 5000
#define MQTT_RECONNECT_TIMEOUT 5000
#define WIFI_CON_DOT_TIMEOUT 500
#define WIFI_RESET_TIMEOUT 1000



///////////////////////////////////////////////////////
////////////////  WI-Fi SETTINGS //////////////////////
///////////////////////////////////////////////////////

#define WIFI_SSID "G"
#define WIFI_PSWD "12344321"

///////////////////////////////////////////////////////
/////////////////  MQTT SETTINGS //////////////////////
///////////////////////////////////////////////////////

#define MQTT_HOST "192.168.96.128"
#define MQTT_PORT 1883
#define MQTT_USER "admin"
#define MQTT_PSWD "12344321"
#define MQTT_DEVICE_ID "esp32_argo-1"

#define MQTT_TEST_TOPIC "test"
#define HELLO_MSG "Hello World!!!"

#define MQTT_SET_SERVO "setServo"


///////////////////////////////////////////////////////
///////////////////////   PINS   //////////////////////
///////////////////////////////////////////////////////

#define TDS_PIN 15//34
#define TURBIDITY_PIN 14
#define DS12B20_1WIRE_BUS 5
#define PH_PIN 16//36

#define CON_UP_PIN 34  // точно вверх 
#define CON_DOWN_PIN 36  // точно вниз
#define BUOYANCY_SERVO_PIN 18




#ifdef ___DEBUG___
  #define DPRINT(X) Serial.print(X)
  #define DPRINTLN(X) Serial.println(X)
#else
  #define DPRINT(X)
  #define DPRINTLN(X)
#endif

#endif