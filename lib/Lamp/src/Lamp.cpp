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
    fade_step.red = red_diff / (float) duration;
    temp_fade_color.red = current_color.red;

    float green_diff = current_color.green - color.green;
    fade_step.green = green_diff / (float) duration;
    temp_fade_color.green = current_color.green;

    float blue_diff = current_color.blue - color.blue;
    fade_step.blue = blue_diff / (float) duration;
    temp_fade_color.blue = current_color.blue;


    for (unsigned long i = 0; i < duration; i++) {
        temp_fade_color.red -= fade_step.red;
        temp_fade_color.green -= fade_step.green;
        temp_fade_color.blue -= fade_step.blue;

        current_color.red = (byte) temp_fade_color.red;
        current_color.green = (byte) temp_fade_color.green;
        current_color.blue = (byte) temp_fade_color.blue;
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

void Lamp::start_breathing() {
    temp_fade_color.red = current_color.red;
    temp_fade_color.green = current_color.green;
    temp_fade_color.blue = current_color.blue;

    float duration = (float) breathe_half_duration;
    fade_step.red = temp_fade_color.red  / duration;
    fade_step.green = temp_fade_color.green  / duration;
    fade_step.blue = temp_fade_color.blue  / duration;

    millis_fade_started = arduino_manager->doMillis();
}

void Lamp::stop_breathing() {
    change_color(current_color);
}

void Lamp::change_color_precision(ColorPrecision color) {
    arduino_manager->doAnalogWrite(red_led_pin, (byte) color.red);
    arduino_manager->doAnalogWrite(green_led_pin, (byte) color.green);
    arduino_manager->doAnalogWrite(blue_led_pin, (byte) color.blue);
}


void Lamp::breathe(){
    temp_fade_color.red -= fade_step.red;
    temp_fade_color.green -= fade_step.green;
    temp_fade_color.blue -= fade_step.blue;

    unsigned long diff_millis = arduino_manager->doMillis() - millis_fade_started;

    // Full breathe done. Set millis to repeat the process.
    if (diff_millis > (breathe_half_duration * 2)) {
        millis_fade_started = arduino_manager->doMillis();
    } else {
        // Light is fading
        if (diff_millis < breathe_half_duration) {
            diff_millis = breathe_half_duration - diff_millis; 
        // Light is increasing intensity
        } else {
            diff_millis -= breathe_half_duration; 
        }

        temp_fade_color.red = fade_step.red * diff_millis;
        temp_fade_color.green = fade_step.green * diff_millis;
        temp_fade_color.blue = fade_step.blue * diff_millis;
        change_color_precision(temp_fade_color);
    }
}

Color Lamp::get_color() {
    return current_color;
}