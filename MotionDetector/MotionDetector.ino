#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "CmdHandler.h"
#include "ThermoHandler.h"
#include "cfg.h"



unsigned long lastMQTTCheck = -5000; //This will force an immediate check on init.
uint32_t lastTemperatureMessage = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Thermometer_DHT11 dht11(ONE_WIRE_PIN);


void initWifi()
{
    Serial.print("\nConnecting to ");
    Serial.println(SSID_TO_CONNECT);

    WiFi.begin(SSID_TO_CONNECT, WIFI_PASS);
    WiFi.hostname(CLIENT_NAME);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}


void checkMQTTConnection()
{
    if (!client.connected())
    {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.print("new connection: ");
            if (client.connect(CLIENT_NAME, MQTT_USER, MQTT_PASSWORD))
            {
                Serial.println("connected");
                client.subscribe(cmdHallwayTopic);
                client.subscribe(cmdOutsideTopic);
                client.subscribe(cmdEnterTopic);
            }
            else
            {
                Serial.print("failed, rc=");
                Serial.println(client.state());
            }
        }
        else
        {
            Serial.println("Not connected to WiFI AP, abandoned connect.");
        }
    }
}


void MQTTcallback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    if (length < MAX_PAYLOAD_LENGTH)
    {
        static char tempPayload[MAX_PAYLOAD_LENGTH];
        memcpy(tempPayload, payload, length);
        tempPayload[length] = '\0';
        Serial.println(tempPayload);
        ProcessMqttPayload(topic, tempPayload);
    }
}


void setup()
{
    Serial.begin(115200);
    Serial.println("Initialising");

    initWifi();

    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(MQTTcallback);

    //OTA setup
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(CLIENT_NAME);
    ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.begin();

    dht11.initialize();
}


void loop()
{
    if (millis() - lastMQTTCheck >= 5000)
    {
        checkMQTTConnection();
        lastMQTTCheck = millis();
    }

    //Handle any pending MQTT messages
    client.loop();

    //Handle any pending OTA SW updates
    ArduinoOTA.handle();

    if (millis() - lastTemperatureMessage > TEMPERATURE_MSG_PERIOD)
    {
        dht11.sendTemperature(temperatureTopic);
        lastTemperatureMessage = millis();
    }

    delay(500);
}


void SendMqtt(const char* topic, const char* response)
{
    client.publish(topic, response);
}
