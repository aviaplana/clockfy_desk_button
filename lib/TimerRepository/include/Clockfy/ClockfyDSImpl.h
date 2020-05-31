#if !defined(CLOCKFY_DS_IMPL_H) && !defined(UNIT_TEST)
#define CLOCKFY_DS_IMPL_H

#define DEBUG_API

#include <WiFiClientSecure.h> 
#include "ClockfyDS.h"
#include "ApiDS.h"
#include "clockfy_configuration.h"

class ClockfyDSImpl: public ClockfyDS, protected ApiDS {
    public:
        ClockfyDSImpl();
        Project** getProjects();
        char* startTimer(char* project_id, char* start_time);

    private:
        BearSSL::WiFiClientSecure getConnectedClient();
        String generateGetRequest(String* endpoint);
        String generatePostRequest(String* endpoint, String* body);
        String generateCommonRequest(String* endpoint);
        Project* parseResponse(Stream* stream);
        Color hexToColor(char* hex);


        const char* host = "api.clockify.me";
        const char* endpoint_prefix = "/api/v1";
        const int port = 443;
        const unsigned int max_retry = 10;
        const unsigned long timeout = 15000;
        const char* fingerprint = "1F F5 7F 61 B4 30 75 92 82 A3 B6 92 39 B7 D7 77 0F 80 67 AE";
};

#endif