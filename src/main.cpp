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
char* timer_id = "";

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

void testing(Color current_color, Color color) {
  float red_diff = (float) current_color.red - (float) color.red;
    float red_step = red_diff / (float) 100;
    float red = (float) current_color.red;
      Serial.printf("current:\tR:%d\tG:%d\tB:%d\n", current_color.red, current_color.green, current_color.blue);
      Serial.printf("Target\tR:%d\tG:%d\tB:%d\n", color.red, color.green, color.blue);

    Serial.printf("red: %f, %f, %f\n", red_diff, red_step, red);

    float green_diff = (float) current_color.green - (float) color.green;
    float green_step = green_diff / (float) 100;
    float green = (float) current_color.green;

    float blue_diff = (float) current_color.blue - (float) color.blue;
    float blue_step = blue_diff / (float) 100;
    float blue = (float) current_color.blue;


    for (unsigned long i = 0; i <= 100; i++) {
        red -= red_step;
        green -= green_step;
        blue -= blue_step;

        current_color.red = (byte) red;
        current_color.green = (byte) green;
        current_color.blue = (byte) blue;
        Serial.printf("\tR:%d\tG:%d\tB:%d\n", current_color.red, current_color.green, current_color.blue);
        delay(1);
    }

}

void change_project() {
  current_project++;
  
  if (current_project >= num_projects) {
    current_project = 0;
  }

  Project* project = timer_repository.getProjectPositon(current_project);

  lamp.change_fading_short(project->color);
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
}

void start_timer(char* project_id) {
  timer_id = timer_repository.startTimer(project_id);
}

void start_stop_timer() {
  Project* project = timer_repository.getProjectPositon(current_project);
  
  if (project != NULL) {
    lamp.short_blink();
    
    if (timer_id == "") {
      start_timer(project->id);
    } else {
      stop_timer();
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