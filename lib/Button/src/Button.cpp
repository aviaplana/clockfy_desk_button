#include <Button.h>

Button::Button(ArduinoInterface* arduino_manager, byte pin): arduino_manager(arduino_manager), pin(pin) { }

void Button::setup() {
    arduino_manager->doPinMode(pin, INPUT_PULLUP);
}

press_type Button::wasPressed() {
    press_type press = NO_PRESS;
    if (arduino_manager->doDigitalRead(pin) == LOW) {
        
        if (millis_last_press == 0) {
            millis_last_press = arduino_manager->doMillis();
        }
    } else if (millis_last_press > 0) {
        const unsigned long diff_millis = arduino_manager->doMillis() - millis_last_press;

        if (diff_millis > millis_debounce)  {
            if (diff_millis > millis_long_press) {
                press = LONG_PRESS;
            } else {
                press = SHORT_PRESS;
            }
        }

        millis_last_press = 0;
    }

    return press;
}