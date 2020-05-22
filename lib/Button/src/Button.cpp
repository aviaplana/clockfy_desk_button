#include <Button.h>

Button::Button(ArduinoInterface* arduino_manager, byte pin): arduino_manager(arduino_manager), pin(pin) { }


void Button::isr() {
    if (arduino_manager->doMillis() - millis_last_isr > millis_debounce) {
        millis_last_isr = arduino_manager->doMillis();
        pressed = true;
    }
}

void Button::setup(void (*isr)(), int interrupt_mode) {
    arduino_manager->doPinMode(pin, INPUT_PULLUP);
    byte interruption_number = arduino_manager->doDigitalPinToInterrupt(pin);
    arduino_manager->doAttachInterrupt(interruption_number, isr, interrupt_mode);
}

bool Button::was_pressed() {
    if (pressed) {
        pressed = false;
        return true;
    }

    return false;
}