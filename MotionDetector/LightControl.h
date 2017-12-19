#pragma once
#include "stdint.h"

class LightControl
{
public:
    LightControl(const uint8_t pin, const char *statusTopic);
    void relayOff(void);
    void relayOn(void);
    void setWriteProtected(bool protect) { writeProtected = protect; }
private:
    const uint8_t pin;
    const char *statusTopic;
    bool writeProtected;
    bool relaySwithedOn;
};

