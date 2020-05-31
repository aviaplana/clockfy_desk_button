#if !defined(API_DS_H) && !defined(UNIT_TEST)
#define API_DS_H
#include <WiFiClient.h> 

class ApiDS {
    protected:
        int readResponseHeaders(WiFiClient* client);
        String getResponseBody(Stream* stream);
        void printResponsePart(char* tag, char* buffer, size_t long_bytes);
        unsigned int processResponse(char* tag, char* buffer, Stream* stream);
        void printErrorMessage(int response_code);
};

#endif