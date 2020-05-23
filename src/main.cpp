#include <Arduino.h>
#include <ArduinoManager.h>
#include <Lamp.h>
#include <Button.h>
#include <TimerRepository.h>

#define RED_LED_PIN D8
#define GREEN_LED_PIN D7
#define BLUE_LED_PIN D6
#define BUTTON_PIN D2

ArduinoManager arduino_manager {};
Lamp lamp { &arduino_manager, RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN };
Button button { &arduino_manager, BUTTON_PIN};
byte current_color = 0;

// "this" can't be passed to the ISR, therefore I can't define it inside the Button class. 
void ICACHE_RAM_ATTR button_isr() {
    button.isr();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting up...");

  lamp.setup();
  button.setup(button_isr, FALLING);
}

Color get_next_color() {
  current_color++;

  if (current_color > 7) {
    current_color = 0;
  }

  return Color {
    current_color & 0b100,
    current_color & 0b010,
    current_color & 0b001,
  };
}

void change_color() {
  Color color = get_next_color();
  Serial.printf("R:%d\tG:%d\tB:%d\n", color.red, color.green, color.blue);

  lamp.change_color(color);
}

void loop() {
  if (button.was_pressed()) {
    change_color();
  }
}