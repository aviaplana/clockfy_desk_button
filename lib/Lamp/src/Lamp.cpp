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

    arduino_manager->doDigitalWrite(red_led_pin, (color.red > 0));
    arduino_manager->doDigitalWrite(green_led_pin, (color.green > 0));
    arduino_manager->doDigitalWrite(blue_led_pin, (color.blue > 0));
}

Color Lamp::get_color() {
    return current_color;
}