#include <Arduino.h>
#include "Trigger.h"


InterruptTrigger::InterruptTrigger(uint8_t pin, void(*intFunction)(void), void(*action)(void), uint8_t mode) :
    pin(pin), intFunction(intFunction), action(action), mode(mode)
{
    pinMode(pin, INPUT);
}


void InterruptTrigger::enable()
{
    attachInterrupt(digitalPinToInterrupt(pin), intFunction, RISING);
}


void InterruptTrigger::disable()
{
    detachInterrupt(pin);
}

void InterruptTrigger::Process()
{
    if (interruptOccured)
    {
        interruptOccured = false;
        action();
    }
}



void TimerTrigger::start()
{
    startedAt = millis();
    enabled = true;
}


void TimerTrigger::setDelay(uint32_t delay)
{
    if (delay > 0 && delay < 3600)
    {
        delayMillis = delay * 1000;
    }
}


void TimerTrigger::Process()
{
    if (enabled && (millis() - startedAt > delayMillis))
    {
        enabled = false;
        action();
    }
}


int32_t TimerTrigger::getRunningTime()
{
    if (enabled)
    {
        return millis() - startedAt;
    }

    return -1;
}
