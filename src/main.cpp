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
bool is_timer_running = false;

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

void change_project() {
  if (!is_timer_running) {
    current_project++;
    
    if (current_project >= num_projects) {
      current_project = 0;
    }

    Project* project = timer_repository.getProjectPositon(current_project);

    lamp.change_fading_short(project->color);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting up...");

  lamp.setup();
  button.setup();
  connect_wifi();

  num_projects = timer_repository.getNumProjects();
  change_project();
}

void stop_timer() {
  if (timer_repository.stopTimer()) {
    is_timer_running = false;
    Serial.println("Timmer stopped");
    lamp.success_blink();
  } else {
    Serial.println("Failed to stop timmer");
    lamp.error_blink();
  }
}

void start_timer(char* project_id) {
  is_timer_running = timer_repository.startTimer(project_id);

  if (is_timer_running) {
    Serial.println("Timer started.");
    lamp.success_blink();
  } else {
    Serial.println("Failed to start timer.\n");
    lamp.error_blink();
  }
}

void start_stop_timer() {
  Project* project = timer_repository.getProjectPositon(current_project);
  
  if (project != NULL) {
    lamp.short_blink();
    
    if (is_timer_running) {
      stop_timer();
    } else {
      start_timer(project->id);
    }
  }
}

void loop() {
  switch (button.was_pressed()) {
    case SHORT_PRESS:
      change_project();
      break;
    
    case LONG_PRESS:
      start_stop_timer();
      break;

    case NO_PRESS:
      break;
    
  }
}