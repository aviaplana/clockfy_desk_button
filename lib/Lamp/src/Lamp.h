#ifndef LAMP_H
#define LAMP_H

#include <types.h>
#include <ArduinoInterface.h>

typedef struct Color {
        byte red;
        byte green;
        byte blue;
} Color;

class Lamp {
    public:
        Lamp(ArduinoInterface* arduino_manager, byte red, byte green, byte blue);
        void setup();
        void change_color(Color color);
        Color get_color();
        
    private:
        ArduinoInterface* arduino_manager;
        const byte red_led_pin;
        const byte green_led_pin;
        const byte blue_led_pin;
        Color current_color = {0, 0, 0};
};

#endif