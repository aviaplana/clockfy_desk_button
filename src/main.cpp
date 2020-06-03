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
#define SYNC_INTERVAL 10000

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
bool waiting_for_response = false;
unsigned long last_sync = 0;

void connect_wifi() {
  wifi.setup();
  wifi.connect();

  Serial.print(F("Connecting to the wifi "));
  Serial.print(wifi.getSSID());

  while (!wifi.isConnected()) {
    Serial.print(F("."));
    delay(200);
  }

  Serial.println(F("connected"));  
}

void change_project() {
  if (!is_timer_running) {
    current_project++;
    
    if (current_project >= num_projects) {
      current_project = 0;
    }

    Project* project = timer_repository.getProjectPositon(current_project);

    lamp.changeFadingShort(project->color);
  }
}

void synchronize() {
  last_sync = millis();

  Project* project = timer_repository.getRunningProject();

  if (project != NULL && !is_timer_running) {
    Serial.println(F("There's a timer running!"));
    is_timer_running = true;
    lamp.changeFadingShort(project->color);
    lamp.startBreathing();
  } else if (project == NULL && is_timer_running) {
      Serial.println(F("Timer stopped!"));
      is_timer_running = false;
      lamp.stopBreathing();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Booting up..."));
  lamp.setup();
  button.setup();
  connect_wifi();
  timer_repository.begin();

  num_projects = timer_repository.getNumProjects();
  change_project();
}

void stop_timer() {
  if (timer_repository.stopTimer()) {
    is_timer_running = false;
    Serial.println(F("Timmer stopped"));
    lamp.stopBreathing();
    lamp.successBlink();
  } else {
    Serial.println(F("Failed to stop timmer"));
    lamp.errorBlink();
  }
}

void start_timer(char* project_id) {
  is_timer_running = timer_repository.startTimer(project_id);

  if (is_timer_running) {
    Serial.println(F("Timer started."));
    lamp.startBreathing();
  } else {
    Serial.println(F("Failed to start timer.\n"));
    lamp.errorBlink();
  }
}

void start_stop_timer() {
  Project* project = timer_repository.getProjectPositon(current_project);
  
  if (project != NULL) {
    lamp.shortBlink();
    
    if (is_timer_running) {
      stop_timer();
    } else {
      start_timer(project->id);
    }
  }
}

void loop() {
  bool should_sync = millis() - last_sync > SYNC_INTERVAL;
  
  if (is_timer_running) {
    lamp.breathe();
    if (should_sync && lamp.breatheCycleJustStarted()) {
      lamp.pauseBreating();
      synchronize();
      lamp.resumeBreathing();
    }
  } else if (should_sync) {
    synchronize();
  }

  
  switch (button.wasPressed()) {
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