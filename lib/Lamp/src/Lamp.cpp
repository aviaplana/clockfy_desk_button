#include "Lamp.h"

Lamp::Lamp(ArduinoInterface* arduino_manager, byte red, byte green, byte blue): 
        arduino_manager(arduino_manager), 
        red_led_pin(red), 
        green_led_pin(green), 
        blue_led_pin(blue) {
} 

void Lamp::setup() {
    arduino_manager->doPinMode(red_led_pin, OUTPUT);
    arduino_manager->doPinMode(green_led_pin, OUTPUT);
    arduino_manager->doPinMode(blue_led_pin, OUTPUT);
}

void Lamp::change_color(Color color) {
    current_color = color;

    arduino_manager->doAnalogWrite(red_led_pin, color.red);
    arduino_manager->doAnalogWrite(green_led_pin, color.green);
    arduino_manager->doAnalogWrite(blue_led_pin, color.blue);
}

void Lamp::change_fading_short(Color color) {
    change_fading(color, short_blink_millis);
}

void Lamp::change_fading(Color color, unsigned long duration) {
    float red_diff = current_color.red - color.red;
    float red_step = red_diff / (float) duration;
    float red = current_color.red;

    float green_diff = current_color.green - color.green;
    float green_step = green_diff / (float) duration;
    float green = current_color.green;

    float blue_diff = current_color.blue - color.blue;
    float blue_step = blue_diff / (float) duration;
    float blue = current_color.blue;


    for (unsigned long i = 0; i < duration; i++) {
        red -= red_step;
        green -= green_step;
        blue -= blue_step;

        current_color.red = (byte) red;
        current_color.green = (byte) green;
        current_color.blue = (byte) blue;
        change_color(current_color);

        arduino_manager->doDelay(1);
    }

    change_color(color);
}

void Lamp::short_blink() {
    blink(short_blink_millis);
}

void Lamp::long_blink() {
    blink(long_blink_millis);
}

void Lamp::blink(unsigned long millis) {
    Color none {0, 0, 0};
    Color color_temp = current_color;

    change_fading(none, millis);
    change_fading(color_temp, millis);
}

void Lamp::success_blink() {
    Color current = current_color;
    Color green {0, 255, 0};
    change_fading_short(green);

    for (int i = 0; i <= 3; i++) {
        blink(fast_blink_millis);
    }

    change_fading_short(current);
}

void Lamp::error_blink() {
    Color current = current_color;
    Color red {255, 0, 0};
    change_fading_short(red);

    for (int i = 0; i <= 2; i++) {
        long_blink();
    }

    change_fading_short(current);
}

Color Lamp::get_color() {
    return current_color;
}