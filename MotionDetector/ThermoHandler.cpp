#include "ThermoHandler.h"
#include "cfg.h"
#include <stdio.h>
#include <string.h>


void addFloatToStr(char* str, const float f, uint8_t num_of_dig_after_point)
{
    uint16_t fraction;
    uint16_t devider = 1;

    while (num_of_dig_after_point--)
    {
        devider *= 10;
    }

    if (f >= 0)
    {
        fraction = static_cast<uint16_t>(f * devider) % devider;
    }
    else
    {
        fraction = static_cast<uint16_t>(-f * devider) % devider;
    }

    sprintf(str + strlen(str), "%d.%d", static_cast<int>(f), fraction);
}


void Thermometer::sendTemperature(const char* topic)
{
    static char msg[20];
    float cur_temp;

    memcpy(msg, "{temp:", 8);
    if (getTemperature(&cur_temp) != STATUS_OK)
    {
        sprintf(msg, "error=%d", error_code);
        SendMqtt(statusEnterTopic, msg);
        return;
    }

    addFloatToStr(msg, cur_temp, 1);
    strcat(msg, "}");

    SendMqtt(topic, msg);
}


Thermometer_DS1820::Thermometer_DS1820(const uint8_t pin) : oneWireBus(pin), instance()
{
    instance.setOneWire(&oneWireBus);
}


void Thermometer_DS1820::initialize()
{
    instance.begin();
}


Status Thermometer_DS1820::getTemperature(float* value)
{
    instance.setResolution(9);
    instance.requestTemperaturesByIndex(0);
    *value = instance.getTempCByIndex(0);

    if (*value == 85.0 || *value == (-127.0))
    {
        return CONNECTION_BROKEN;
    }

    return STATUS_OK;
}


Thermometer_DHT11::Thermometer_DHT11(uint8_t pin) : instance(ONE_WIRE_PIN, DHT11)
{
}


Status Thermometer_DHT11::getTemperature(float * value)
{
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = instance.readHumidity();
    // Read temperature as Celsius (the default)
    *value = instance.readTemperature();

    if (*value == 85.0 || *value == (-127.0))
    {
        return CONNECTION_BROKEN;
    }

    return STATUS_OK;
}


void Thermometer_DHT11::initialize()
{
    instance.begin();
}
