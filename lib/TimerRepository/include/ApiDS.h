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

    private:
        BearSSL::WiFiClientSecure getConnectedClient();
        String getRequest(String* endpoint);
        String getRequest(String* endpoint, String* headers);
        void readResponseHeaders(WiFiClientSecure* client);
        String getResponseBody(WiFiClientSecure* client);
        void printResponsePart(char* tag, char* buffer, size_t long_bytes);
        Project* parseResponse(char* buffer, Stream* stream);
        unsigned int processResponse(char* tag, char* budder, Stream* stream);
        Color hexToColor(char* hex);


        const char* host = "api.clockify.me";
        const char* endpoint_prefix = "/api/v1";
        const int port = 443;
        const unsigned int max_retry = 10;
        const unsigned long timeout = 15000;
        const char* fingerprint = "C1 3A B9 2C 13 FF F0 24 B1 7B 8F A0 4D AA AD 67 CE 7C 9D 69";
};

#endif