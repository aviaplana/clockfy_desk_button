#define DEBUG_API 1

#include <Arduino.h>
#include <ArduinoManager.h>
#include <Lamp.h>
#include <Button.h>
#include <TimerRepository.h>
#include <LocalDS.h>
#include <Clockfy/ClockfyDSImpl.h>
#include <DateTime/DateTimeDSImpl.h>
#include <WifiManager.h>

#define RED_LED_PIN D8
#define GREEN_LED_PIN D7
#define BLUE_LED_PIN D6
#define BUTTON_PIN D2

WifiManager wifi;
ArduinoManager arduino_manager {};
DateTimeDSImpl datetime_ds {};
LocalDS local_ds {};
ClockfyDSImpl clockfy_ds {};

Lamp lamp { &arduino_manager, RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN };
Button button { &arduino_manager, BUTTON_PIN};
TimerRepository timer_repository {&local_ds, &clockfy_ds, &datetime_ds};

byte num_projects = 0;
byte current_project = 0;

void connect_wifi() {
  wifi.setup();
  wifi.connect();

  Serial.print("Connecting to the wifi ");
  Serial.print(wifi.getSSID());

  while (!wifi.is_connected()) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("connected");  
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting up...");

  lamp.setup();
  button.setup();
  connect_wifi();

  num_projects = timer_repository.getNumProjects();
}

void change_project() {
  current_project++;
  
  if (current_project >= num_projects) {
    current_project = 0;
  }

  Project* project = timer_repository.getProjectPositon(current_project);
  Serial.printf("%s - R:%d\tG:%d\tB:%d\n", project->name, project->color.red, project->color.green, project->color.blue);
  lamp.change_color(project->color);
}

void loop() {
  switch (button.was_pressed()) {
    case SHORT_PRESS:
      change_project();
      break;
    
    case LONG_PRESS:
      // TODO: Start / stop timer
      break;

    case NO_PRESS:
      break;
    
  }
}