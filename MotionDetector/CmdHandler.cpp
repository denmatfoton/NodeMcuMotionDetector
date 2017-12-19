#include <string.h>
#include "Arduino.h"
#include "CmdHandler.h"
#include "LightControl.h"
#include "Trigger.h"


const char *cmdHallwayTopic = CMD_HALLWAY_TOPIC;
const char *statusHallwayTopic = STATUS_HALLWAY_TOPIC;
const char *cmdOutsideTopic = CMD_OUTSIDE_TOPIC;
const char *statusOutsideTopic = STATUS_OUTSIDE_TOPIC;
const char *cmdEnterTopic = CMD_ENTER_LIGHT_TOPIC;
const char *statusEnterTopic = STATUS_ENTER_TOPIC;
const char *temperatureTopic = TEMPERATURE_TOPIC;


void motionInterrupt(void);
void motionAction(void);
void motionTimerAction(void);
void doorInterrupt(void);
void doorAction(void);
void doorTimerAction(void);

InterruptTrigger motionTrigger(MOTION_DETECTOR_PIN, motionInterrupt, motionAction);
TimerTrigger motionTimer(motionTimerAction);

InterruptTrigger doorTrigger(DOOR_OPEN_PIN, doorInterrupt, doorAction, CHANGE);
TimerTrigger doorTimer(doorTimerAction);

LightControl hallwayLight(HALLWAY_RELAY_PIN, statusHallwayTopic);
LightControl outsideLight(OUTSIDE_RELAY_PIN, statusOutsideTopic);
LightControl livingRoomLight(LIVING_ROOM_RELAY_PIN, statusEnterTopic);


void motionInterrupt(void)
{
    motionTrigger.setInterruptOccured();
}

void motionAction(void)
{
    motionTimer.start();
    hallwayLight.relayOn();
}

void motionTimerAction(void)
{
    hallwayLight.relayOff();
}


void doorInterrupt(void)
{
    doorTrigger.setInterruptOccured();
}

void doorAction(void)
{
    if (DOOR_OPEN_VALUE == digitalRead(DOOR_OPEN_PIN))
    {
        if (motionTimer.getRunningTime() > 1000)
        {
            doorTimer.start();
            livingRoomLight.relayOn();
        }
        SendMqtt(statusEnterTopic, "open");
    }
    else
    {
        SendMqtt(statusEnterTopic, "close");
    }
}

void doorTimerAction(void)
{
    livingRoomLight.relayOff();
}


void ProcessMqttPayload(const char* topic, const char* payload)
{
    if (strncmp(topic, cmdHallwayTopic, strlen(cmdHallwayTopic)) == 0)
    {
        if (strncmp(payload, "on", 2) == 0)
        {
            hallwayLight.relayOn();
            hallwayLight.setWriteProtected(true);
        }
        else if (strncmp(payload, "off", 3) == 0)
        {
            hallwayLight.setWriteProtected(false);
            hallwayLight.relayOff();
        }
        else if (strncmp(payload, "autoOn", 6) == 0)
        {
            motionTrigger.enable();
            SendMqtt(statusHallwayTopic, "autoOn");
        }
        else if (strncmp(payload, "autoOff", 7) == 0)
        {
            motionTrigger.disable();
            SendMqtt(statusHallwayTopic, "autoOff");
        }
        else if (strncmp(payload, "delay", 5) == 0)
        {
            int newVal = atoi(payload + 5);
            motionTimer.setDelay(newVal);
        }
    }
    else if (strncmp(topic, cmdOutsideTopic, strlen(cmdOutsideTopic)) == 0)
    {
        if (strncmp(payload, "on", 2) == 0)
        {
            outsideLight.relayOn();
        }
        else if (strncmp(payload, "off", 3) == 0)
        {
            outsideLight.relayOff();
        }
    }
    else if (strncmp(topic, cmdEnterTopic, strlen(cmdEnterTopic)) == 0)
    {
        if (strncmp(payload, "on", 2) == 0)
        {
            livingRoomLight.relayOn();
            livingRoomLight.setWriteProtected(true);
        }
        else if (strncmp(payload, "off", 3) == 0)
        {
            livingRoomLight.setWriteProtected(false);
            livingRoomLight.relayOff();
        }
        else if (strncmp(payload, "autoOn", 6) == 0)
        {
            livingRoomLight.setWriteProtected(false);
            SendMqtt(statusEnterTopic, "autoOn");
        }
        else if (strncmp(payload, "autoOff", 7) == 0)
        {
            livingRoomLight.relayOff();
            livingRoomLight.setWriteProtected(true);
            SendMqtt(statusEnterTopic, "autoOff");
        }
        else if (strncmp(payload, "delay", 5) == 0)
        {
            int newVal = atoi(payload + 5);
            doorTimer.setDelay(newVal);
        }
    }
}


void initPeripherals()
{
    doorTrigger.enable();
}
