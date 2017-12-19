#pragma once
#include <stdint.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <DHT.h>


enum Status
{
    STATUS_OK = 0,
    ADDRESS_NOT_FOUND,
    CRC_ERROR,
    UNKNOWN_DEVICE,
    CONNECTION_BROKEN
};


class Thermometer
{
public:
    virtual Status getTemperature(float* value) = 0;
    virtual void initialize() = 0;
    void sendTemperature(const char* topic);
protected:
    Status error_code;
    bool initialized = false;
};


class Thermometer_DS1820 : public Thermometer
{
public:
    Thermometer_DS1820(uint8_t pin);
    Status getTemperature(float* value) override;
    void initialize() override;
private:
    OneWire oneWireBus;
    DallasTemperature instance;
};


class Thermometer_DHT11 : public Thermometer
{
public:
    Thermometer_DHT11(uint8_t pin);
    Status getTemperature(float* value) override;
    void initialize() override;
private:
    DHT instance;
};

