#include <Arduino.h>
#include "Lamp.h"

#define RED_LED_PIN D8
#define GREEN_LED_PIN D7
#define BLUE_LED_PIN D6
#define BUTTON_PIN D2

Lamp lamp { RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN };
byte current_color = 0;
bool button_flag = false;
unsigned long millis_last_button_interrupt = 0;
const unsigned long interruption_debounce_millis = 200;

void ICACHE_RAM_ATTR button_isr() {
  if (millis() - millis_last_button_interrupt > interruption_debounce_millis) {
    millis_last_button_interrupt = millis();
    button_flag = true;
  }
}

void setup_button() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_isr, FALLING);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting up...");
  lamp.setup();
  setup_button();
}

void change_color() {
  Serial.println("Changing color!");
  current_color++;

  if (current_color > 7) {
    current_color = 0;
  }

  Color color {
    current_color & 0b100,
    current_color & 0b010,
    current_color & 0b001,
  };

  lamp.change_color(color);
  button_flag = false;
}

void loop() {
  if (button_flag) {
    change_color();
  }

  delay(100);
}