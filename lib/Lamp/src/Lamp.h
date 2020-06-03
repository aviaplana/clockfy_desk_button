#ifndef LAMP_H
#define LAMP_H

#include <types.h>
#include <ArduinoInterface.h>

class Lamp {
    public:
        Lamp(ArduinoInterface* arduino_manager, byte red, byte green, byte blue);
        void setup();
        void changeColor(Color color);
        void changeFadingShort(Color color);
        void changeFading(Color color, unsigned long duration);
        Color getColor();
        void shortBlink();
        void longBlink();
        void successBlink();
        void errorBlink();
        void breathe();
        void startBreathing();
        void stopBreathing();
        bool isPaused();
        void resumeBreathing();
        void pauseBreating();
        bool breatheCycleJustStarted();

    private:
        void blink(unsigned long millis);
        void changeColorPrecision(ColorPrecision color);

        const unsigned long breathe_half_duration = 3000;
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
        unsigned long millis_breathing_paused = 0;
        bool is_paused = false;
};

#endif