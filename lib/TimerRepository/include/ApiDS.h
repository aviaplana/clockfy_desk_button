#if !defined(API_DS_H) && !defined(UNIT_TEST)
#define API_DS_H

#define DEBUG_API

#include <WiFiClientSecure.h> 
#include "DataSource.h"
#include "api_configuration.h"

class ApiDS: public DataSource {
    public:
        ApiDS();
        Project** getProjects();
        char* startTimer(char* project_id, char* start_time);

    private:
        BearSSL::WiFiClientSecure getConnectedClient();
        String generateGetRequest(String* endpoint);
        String generatePostRequest(String* endpoint, String* body);
        String generateCommonRequest(String* endpoint);
        int readResponseHeaders(WiFiClientSecure* client);
        String getResponseBody(WiFiClientSecure* client);
        void printResponsePart(char* tag, char* buffer, size_t long_bytes);
        Project* parseResponse(Stream* stream);
        unsigned int processResponse(char* tag, char* budder, Stream* stream);
        Color hexToColor(char* hex);


        const char* host = "api.clockify.me";
        const char* endpoint_prefix = "/api/v1";
        const int port = 443;
        const unsigned int max_retry = 10;
        const unsigned long timeout = 15000;
        const char* fingerprint = "1F F5 7F 61 B4 30 75 92 82 A3 B6 92 39 B7 D7 77 0F 80 67 AE";
};

#endif