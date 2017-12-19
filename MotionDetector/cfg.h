#ifndef CFG_H
#define CFG_H
#include <stdint.h>

#define UART_BAUD_RATE    115200

#define CLIENT_NAME       "HallwayController"
#define SSID_TO_CONNECT   "WiFi"
#define WIFI_PASS         "Password"

#define OTA_PASS          "UPDATE_PW"
#define OTA_PORT          8266

#define MQTT_SERVER       "192.168.0.1"
#define MQTT_PORT         1883
#define MQTT_USER         "user"
#define MQTT_PASSWORD     "Password"



#define MAX_PAYLOAD_LENGTH    100
#define CMD_HALLWAY_TOPIC     "myhome/hallway"
#define CMD_OUTSIDE_TOPIC     "myhome/outside/light"
#define CMD_ENTER_LIGHT_TOPIC "myhome/enter/light"
#define STATUS_HALLWAY_TOPIC  "myhome/hallway/status"
#define STATUS_OUTSIDE_TOPIC  "myhome/outside/status"
#define STATUS_ENTER_TOPIC    "myhome/enter/status"
#define TEMPERATURE_TOPIC     "myhome/livingroom/temphum"

extern const char *cmdHallwayTopic;
extern const char *statusHallwayTopic;
extern const char *cmdOutsideTopic;
extern const char *statusOutsideTopic;
extern const char *cmdEnterTopic;
extern const char *statusEnterTopic;
extern const char *temperatureTopic;


#define MOTION_DETECTOR_PIN   D1
#define DOOR_OPEN_PIN         D2
#define ONE_WIRE_PIN          D3

#define HALLWAY_RELAY_PIN     D4
#define OUTSIDE_RELAY_PIN     D5
#define LIVING_ROOM_RELAY_PIN D6

#define DOOR_OPEN_VALUE       0

#define TEMPERATURE_MSG_PERIOD     10000 // 10 s


void SendMqtt(const char* topic, const char* response);

#endif // CFG_H
