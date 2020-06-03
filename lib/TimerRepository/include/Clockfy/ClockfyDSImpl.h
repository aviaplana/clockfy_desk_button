#if !defined(CLOCKFY_DS_IMPL_H) && !defined(UNIT_TEST)
#define CLOCKFY_DS_IMPL_H

#define DEBUG_API

#include <WiFiClientSecure.h> 
#include "ClockfyDS.h"
#include "clockfy_configuration.h"

class ClockfyDSImpl: public ClockfyDS {
    public:
        ClockfyDSImpl();
        Project** getProjects(char* workspace_id);
        bool startTimer(char* project_id, char* workspace_id, char* start_time);
        bool stopTimer(char* stop_time, char* user_id, char* workspace_id);        
        UserData* getUserData();
        char* getCurrentTimer(char* user_id, char* workspace_id);

    private:
        typedef enum { 
            NOT_FOUND = 404, 
            FORBIDDEN = 403, 
            UNAUTHORIZED = 401, 
            BAD_REQUEST = 400, 
            NO_CONTENT = 204, 
            CREATED = 201, 
            OK = 200 
        } response_codes;
        
        void flushBuffers();
        int readResponseHeaders();
        String getResponseBody();
        void printResponsePart(char* tag, char* buffer, size_t long_bytes);
        unsigned int processResponse(char* tag, char* buffer);
        void printErrorMessage(int response_code);
        String generateBodyRequest(String* body);
        bool isHttpsClientReadyToUse();
        String generateGetRequest(String* endpoint);
        String generatePostRequest(String* endpoint, String* body);
        String generatePatchRequest(String* endpoint, String* body);
        String generateCommonRequest(String* endpoint);
        Project* parseResponseToProject();
        Color hexToColor(char* hex);
        void printProject(Project* project);
        void sincronizeTime();

        BearSSL::WiFiClientSecure httpsClient;
        const char* host = "api.clockify.me";
        const char* endpoint_prefix = "/api/v1";
        const int port = 443;
        const unsigned int max_retry = 10;
        const unsigned long timeout = 15000;
};

#endif