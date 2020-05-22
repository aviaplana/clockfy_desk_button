#ifndef UNIT_TEST 
#include <ArduinoManager.h>

void ArduinoManager::doDigitalWrite(uint8_t pin, uint8_t val) {
    digitalWrite(pin, val);
}

int ArduinoManager::doDigitalRead(uint8_t pin) {
    return digitalRead(pin);
}

void ArduinoManager::doPinMode(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);
}

uint8_t ArduinoManager::doDigitalPinToInterrupt(uint8_t pin) {
    return digitalPinToInterrupt(pin);
}

void ArduinoManager::doAttachInterrupt(uint8_t interruption_number, void (*isr)(), uint8_t mode) {
    attachInterrupt(interruption_number, isr, mode);
}

unsigned long ArduinoManager::doMillis() {
    return millis();
}

#endif