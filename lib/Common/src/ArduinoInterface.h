#ifndef ARDUINO_INTERFACE_H
#define ARDUINO_INTERFACE_H

#include "types.h"

#define HIGH 0x1
#define LOW  0x0

//GPIO FUNCTIONS
#define INPUT             0x00
#define INPUT_PULLUP      0x02
#define INPUT_PULLDOWN_16 0x04 // PULLDOWN only possible for pin16
#define OUTPUT            0x01
#define OUTPUT_OPEN_DRAIN 0x03
#define WAKEUP_PULLUP     0x05
#define WAKEUP_PULLDOWN   0x07

//Interrupt Modes
#define RISING    0x01
#define FALLING   0x02
#define CHANGE    0x03
#define ONLOW     0x04
#define ONHIGH    0x05
#define ONLOW_WE  0x0C
#define ONHIGH_WE 0x0D

class ArduinoInterface {
public:
    virtual void doDigitalWrite(uint8_t pin, uint8_t val) = 0;
    virtual void doAnalogWrite(uint8_t pin, uint8_t val) = 0;
    virtual int doDigitalRead(uint8_t pin) = 0;
    virtual void doPinMode(uint8_t pin, uint8_t mode) = 0;
    virtual uint8_t doDigitalPinToInterrupt(uint8_t pin) = 0;
    virtual void doAttachInterrupt(uint8_t interruption_number, void (*isr)(), uint8_t mode) = 0;
    virtual unsigned long doMillis() = 0;
    virtual void doDelay(unsigned long time) = 0;
};

#endif