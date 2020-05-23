#include "Wifi.h"

Wifi::Wifi() {
}

void Wifi::connect() {
    WiFi.begin(ssid, password);
}

bool Wifi::is_connected() {
    return WiFi.status() == WL_CONNECTED;
}
