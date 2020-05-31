#ifndef ARDUINO_MANAGER_H
#define ARDUINO_MANAGER_H

#include <Arduino.h>
#include "ArduinoInterface.h"

class ArduinoManager: public ArduinoInterface {
public:
    void doDigitalWrite(uint8_t pin, uint8_t val);
    void doAnalogWrite(uint8_t pin, uint8_t val);
    int doDigitalRead(uint8_t pin);
    void doPinMode(uint8_t pin, uint8_t mode);
    uint8_t doDigitalPinToInterrupt(uint8_t pin);
    void doAttachInterrupt(uint8_t interruption_number, void (*isr)(), uint8_t mode);
    unsigned long doMillis();
    void doDelay(unsigned long millis);

};

#endif