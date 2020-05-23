#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include "wifi_configuration.h"

class Wifi {
    public:
        Wifi();
        void connect();
        bool is_connected();

    private:
};

#endif