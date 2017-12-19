#include <Arduino.h>
#include "LightControl.h"
#include "cfg.h"


LightControl::LightControl(const uint8_t pin, const char *statusTopic) : pin(pin), relaySwithedOn(false),
                                                writeProtected(false), statusTopic(statusTopic)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}


void LightControl::relayOff(void)
{
    if (relaySwithedOn && !writeProtected)
    {
        digitalWrite(pin, LOW);
        relaySwithedOn = false;
        SendMqtt(statusTopic, "on");
    }
}

void LightControl::relayOn(void)
{
    if (!relaySwithedOn && !writeProtected)
    {
        digitalWrite(pin, HIGH);
        relaySwithedOn = true;
        SendMqtt(statusTopic, "off");
    }
}
