#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>

#include "wifi_mqtt.h"

#include "settings.h"
#include "mqtt_handlers.h"


void callback(char* topic, byte* message, unsigned int length);
 

WiFiClient espClient;

PubSubClient client(MQTT_HOST,
                    MQTT_PORT,
                    callback,
                    espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

bool wifiMode = WIFI_IS_ON;

bool get_wifiMode(){
    return wifiMode;
}

void setWifiMode(bool new_wifiMode){
    wifiMode = new_wifiMode;
}


bool mqttMode = MQTT_IS_ON;

bool get_mqttMode(){
    return mqttMode;
}

void setMqttMode(bool new_mqttMode){
    mqttMode = new_mqttMode;
}


bool check_wifiConnection(){
    return (WiFi.status() == WL_CONNECTED);
}

void stop_wifi(){
    WiFi.disconnect();
}

void setup_wifi(){
    if (get_wifiMode() == WIFI_IS_OFF){
        return;
    }
    
    delay(10);

    DPRINT("\nConnecting to ");
    DPRINTLN(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PSWD);

    while (!check_wifiConnection()) {
        delay(WIFI_CON_DOT_TIMEOUT);
        DPRINT(".");
    }

    DPRINTLN((String)"\n" + 
            "WiFi connected\n" +
            "IP address: " + 
            String(WiFi.localIP()));  /// Bug with IP print!
}




void mqtt_send_sensorData(String msg){
    client.publish(MQTT_TEST_TOPIC, msg.c_str());
}

bool check_mqtt_connection(){
    return client.connected();
}

void mqtt_tick(){
    client.loop();
}

bool mqtt_connect(){
    bool result = 0;
    if (client.connect(MQTT_DEVICE_ID, MQTT_USER, MQTT_PSWD)) {
        client.publish(MQTT_TEST_TOPIC, HELLO_MSG);
        client.subscribe(MQTT_TEST_TOPIC);
        client.subscribe(MQTT_SET_SERVO);
        result = 1;
    }
    return result;
}

void mqtt_routine(){
  if (!check_mqtt_connection()) {
    mqtt_reconnect();
  }
  mqtt_tick();
}

void mqtt_reconnect() {
    if (!check_wifiConnection()){
        stop_wifi();
        delay(WIFI_RESET_TIMEOUT);
        setup_wifi();
    }
    bool fl = 0;
    while (!fl) {
        DPRINT("Attempting MQTT connection...");

        if (mqtt_connect()) {
            fl = 1;
            DPRINTLN("connected!\nsubscribed!");
        } else {
            DPRINTLN((String)"failed, rc=" + 
                    client.state() + 
                    "try again in 5 seconds");
            delay(MQTT_RECONNECT_TIMEOUT);
        }
    }
}





void callback(char* topic, byte* message, unsigned int length){
    String topicStr = topic;

    DPRINT((String)"Message arrived on topic: " +
            topic + 
            ". Message: ");

    String msgStr;

    //// How to convert byte string to String?
    for (int i = 0; i < length; i++) {
        DPRINT((char)message[i]);
        msgStr += (char)message[i];
    }

    if (topicStr == MQTT_SET_SERVO){
        mqtt_setServo_handler(msgStr);
    }
}


