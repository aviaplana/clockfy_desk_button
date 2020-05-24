#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>

class WifiManager {
    public:
        WifiManager();
        void setup();
        void connect();
        bool is_connected();
        const char* getSSID();
};

#endif