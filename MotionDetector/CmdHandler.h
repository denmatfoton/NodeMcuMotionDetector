#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H
#include "cfg.h"


void ProcessMqttPayload(const char* topic, const char* payload);
void initPeripherals();


#endif // CMD_HANDLER_H
