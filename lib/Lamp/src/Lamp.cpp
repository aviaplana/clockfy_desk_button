#include "Lamp.h"

Lamp::Lamp(byte red, byte green, byte blue): red_led_pin(red), green_led_pin(green), blue_led_pin(blue) {

} 

void Lamp::setup() {
    pinMode(red_led_pin, OUTPUT);
    pinMode(green_led_pin, OUTPUT);
    pinMode(blue_led_pin, OUTPUT);
}

void Lamp::change_color(Color color) {
    current_color = color;

    digitalWrite(red_led_pin, (color.red > 0));
    digitalWrite(green_led_pin, (color.green > 0));
    digitalWrite(blue_led_pin, (color.blue > 0));
}

Color Lamp::get_color() {
    return current_color;
}