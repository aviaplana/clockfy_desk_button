#include "WifiManager.h"
#include "wifi_configuration.h"

WifiManager::WifiManager() {
    WiFi.mode(WIFI_STA);
}

void WifiManager::setup() {
    WiFi.mode(WIFI_OFF); //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
}

const char* WifiManager::getSSID() {
    return WIFI_SSID;
}

void WifiManager::connect() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool WifiManager::is_connected() {
    return WiFi.status() == WL_CONNECTED;
}