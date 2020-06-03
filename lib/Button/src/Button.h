#ifndef BUTTON_H
#define BUTTON_H

#include <ArduinoInterface.h>
#include "types.h"

typedef enum {NO_PRESS = 0, SHORT_PRESS = 1, LONG_PRESS = 2} press_type;

class Button {
    public:
        Button(ArduinoInterface* arduino_manager, byte pin);
        void setup();
        press_type wasPressed();

    private:
        ArduinoInterface* arduino_manager;
        const byte pin;
        const unsigned long millis_debounce = 50;
        const unsigned long millis_long_press = 500;
        press_type pressed = NO_PRESS;
        volatile unsigned long millis_last_press = 0;
};

#endif