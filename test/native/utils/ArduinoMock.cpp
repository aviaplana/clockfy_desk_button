#include "ArduinoInterface.h"

class MockArduino: public ArduinoInterface {
    private:
        struct Pin {
            uint8_t pin;
            unsigned short value;
            uint8_t mode;
        };
        Pin pins[5];
        uint8_t num_pins = 0;

        int getPinPosition(uint8_t pin) {
            for(uint8_t i = 0; i < num_pins; i++) {
                if (pin == pins[i].pin) {
                    return i;
                }
            }

            return -1;
        }

    public:
        void doPinMode(uint8_t pin, uint8_t mode) {
            pins[num_pins] = (Pin) {
                pin,
                0,
                mode
            };
            num_pins++;
        }

        void doDigitalWrite(uint8_t pin, uint8_t val) {
            int pin_pos = getPinPosition(pin);

            if (pin_pos >= 0) {
                if (val > 0) {
                    pins[pin_pos].value = HIGH;
                } else {
                    pins[pin_pos].value = LOW;
                }
            }
        }

        void doAnalogWrite(uint8_t pin, uint8_t val) {
            int pin_pos = getPinPosition(pin);

            if (pin_pos >= 0) {
                pins[pin_pos].value = val;
            }
        }

        int doDigitalRead(uint8_t pin) {
            int pin_pos = getPinPosition(pin);

            if (pin_pos >= 0) {
                if (pins[pin_pos].value > 0) {
                    return HIGH;
                } else {
                    return LOW;
                }
            }

            return -1;
        }
        
        int doAnalogRead(uint8_t pin) {
            int pin_pos = getPinPosition(pin);

            if (pin_pos >= 0) {
                return pins[pin_pos].value;
            }

            return -1;
        }


        uint8_t doDigitalPinToInterrupt(uint8_t pin) { return 1; }

        void doAttachInterrupt(uint8_t interruption_number, void (*isr)(), uint8_t mode) { };
    
        unsigned long doMillis() { return 1000; }
};