#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "types.h"

class Button {
    public:
        Button(byte pin);
        void setup(void (*isr)(), int interrupt_mode);
        bool was_pressed();
        void isr();

    private:
        void button_isr();
        const byte pin;
        const unsigned long millis_debounce = 200;
        bool pressed = false;
        unsigned long millis_last_isr = 0;

};

#endif