
[![Build Status](https://travis-ci.com/aviaplana/clockfy_desk_button.svg?token=xgcoVHECpWhjPPHdzKr9&branch=master)](https://travis-ci.com/aviaplana/clockfy_desk_button)

  
  

## What is it?

It's a physical device used to start and stop timers from clockfy. 

  

## Why do I need it?

I found myself using the clockfy webapp to start timers, but I kept on forgetting to stop them. I wanted to have a physical device to remid me that a timer was running, so I sarted to work on this project.

  
## How to build the circuit?
Components needed:
- 3x 82ohm resistors.
- Blue LED
- Red LED
- Green LED
- Pushbutton
- ESP8266 MCU
  
Connections:
- Red LED -> D8
- Green LED -> D7
- Blue LED -> D6
- Buttton -> D2

![The circuit](/circuit.jpg)


## How to install it?
Build it using the [PlatforIO IDE](https://platformio.org/). 

Before burning the firmware to the MCU, 2 configuration files must be created:

*lib/WifiManager/src/wifi_configuration.h* - Contains the wifi credentials.

*lib/TimerRepository/include/Clockfy/clockfy_configuration.h* - contains the Clockfy API key.

## How to use it?

__Short press to select a project.__ The RGB led will match the color that is associated with the project.

__Long press to start a timer.__ The light blinks to let the user know that the request has been sent. If there's no errors, it will change color to green, and blink 3 times rapidly. On the other hand, if something goes wrong, it will change to red and blink twice.

__Long press to stop timer.__ Again, short green blinks to indicate that everything went ok, and long red blikns to indicate that something went wrong.
  

## Apis

__http://worldtimeapi.org/__ to get current datetime

__https://api.clockfy.me__ to update timers.


<br />
<br />
<br />
<br />

_An api key might be found across commit history. It is no longer valid._

 
