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
        Project** getProjects(char* workspace_id);
        bool startTimer(char* project_id, char* workspace_id, char* start_time);
        bool stopTimer(char* stop_time, char* user_id, char* workspace_id);        
        UserData* getUserData();

    private:
        BearSSL::WiFiClientSecure getConnectedClient();
        String generateGetRequest(String* endpoint);
        String generatePostRequest(String* endpoint, String* body);
        String generatePatchRequest(String* endpoint, String* body);
        String generateCommonRequest(String* endpoint);
        Project* parseResponse(Stream* stream);
        Color hexToColor(char* hex);
        void printProject(Project* project);
        void sincronizeTime();

        const char* host = "api.clockify.me";
        const char* endpoint_prefix = "/api/v1";
        const int port = 443;
        const unsigned int max_retry = 10;
        const unsigned long timeout = 15000;
};

#endif