#if !defined(API_DS_H) && !defined(UNIT_TEST)
#define API_DS_H
#define DEBUG_API

#include <WiFiClient.h> 

class ApiDS {
    protected:
        typedef enum { 
            NOT_FOUND = 404, 
            FORBIDDEN = 403, 
            UNAUTHORIZED = 401, 
            BAD_REQUEST = 400, 
            NO_CONTENT = 204, 
            CREATED = 201, 
            OK = 200 
        } response_codes;

        int readResponseHeaders(WiFiClient* client);
        String getResponseBody(Stream* stream);
        void printResponsePart(char* tag, char* buffer, size_t long_bytes);
        unsigned int processResponse(char* tag, char* buffer, Stream* stream);
        void printErrorMessage(int response_code);
        String generateBodyRequest(String* body);
};

#endif