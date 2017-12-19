#pragma once
#include "stdint.h"


class Trigger
{
public:
    void Process() {}
protected:
};


class InterruptTrigger : public Trigger
{
public:
    InterruptTrigger(uint8_t pin, void(*intFunction)(void), void(*action)(void), uint8_t mode = RISING);
    void enable();
    void disable();
    void setInterruptOccured() { interruptOccured = true; }
    void Process();
private:
    const uint8_t pin;
    void(*const intFunction)(void);
    void(*action)(void);
    const uint8_t mode;
    bool interruptOccured;
};


class TimerTrigger : public Trigger
{
public:
    TimerTrigger(void(*action)(void), uint32_t delayMillis = 60000) :
        action(action), delayMillis(delayMillis), enabled(false) {}
    void start();
    void setDelay(uint32_t delay);
    void Process();
    int32_t getRunningTime();
private:
    void(*action)(void);
    bool enabled;
    uint32_t startedAt;
    uint32_t delayMillis;
};
