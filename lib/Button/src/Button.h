#ifndef BUTTON_H
#define BUTTON_H

#include <ArduinoInterface.h>
#include "types.h"

class Button {
    public:
        Button(ArduinoInterface* arduino_manager, byte pin);
        void setup(void (*isr)(), int interrupt_mode);
        bool was_pressed();
        void isr();

    private:
        void button_isr();

        ArduinoInterface* arduino_manager;
        const byte pin;
        const unsigned long millis_debounce = 200;
        bool pressed = false;
        unsigned long millis_last_isr = 0;

};

#endif