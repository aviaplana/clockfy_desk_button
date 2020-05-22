#include <Button.h>

Button::Button(byte pin): pin(pin) { }


void Button::isr() {
    if (millis() - millis_last_isr > millis_debounce) {
        millis_last_isr = millis();
        pressed = true;
    }
}

void Button::setup(void (*isr)(), int interrupt_mode) {
    pinMode(pin, INPUT_PULLUP);
    byte interruption_number = digitalPinToInterrupt(pin);
    attachInterrupt(interruption_number, isr, interrupt_mode);
}

bool Button::was_pressed() {
    if (pressed) {
        pressed = false;
        return true;
    }

    return false;
}