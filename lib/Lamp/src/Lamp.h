#ifndef LAMP_H
#define LAMP_H

#include <Arduino.h>
#include "types.h"

typedef struct Color {
        byte red;
        byte green;
        byte blue;
} Color;

class Lamp {
    public:
        Lamp(byte red, byte green, byte blue);
        void setup();
        void change_color(Color color);
        Color get_color();
        
    private:
        const byte red_led_pin;
        const byte green_led_pin;
        const byte blue_led_pin;
        Color current_color = Color();
};

#endif