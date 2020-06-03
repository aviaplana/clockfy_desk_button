#ifndef LAMP_H
#define LAMP_H

#include <types.h>
#include <ArduinoInterface.h>

class Lamp {
    public:
        Lamp(ArduinoInterface* arduino_manager, byte red, byte green, byte blue);
        void setup();
        void change_color(Color color);
        void change_fading_short(Color color);
        void change_fading(Color color, unsigned long duration);
        Color get_color();
        void short_blink();
        void long_blink();
        void success_blink();
        void error_blink();
        void breathe();
        void start_breathing();
        void stop_breathing();
        
    private:
        void blink(unsigned long millis);
        void change_color_precision(ColorPrecision color);

        const unsigned long breathe_duration = 3000;
        const unsigned long fast_blink_millis = 100;
        const unsigned long short_blink_millis = 200;
        const unsigned long long_blink_millis = 500;
        ArduinoInterface* arduino_manager;
        const byte red_led_pin;
        const byte green_led_pin;
        const byte blue_led_pin;
        Color current_color = {0, 0, 0};
        ColorPrecision temp_fade_color = {0.0f, .0f, 0.0f};
        ColorPrecision fade_step = {0.0f, .0f, 0.0f};
        unsigned long millis_fade_started = 0;
};

#endif