#ifndef UNIT_TEST

#include "ApiDS.h"

int ApiDS::readResponseHeaders(WiFiClient* client) {
    #ifdef DEBUG_API
        Serial.println(F("Response headers:"));
    #endif
    char* response_code = (char*) malloc(10);

    client->find("HTTP");
    client->find(" ");
    size_t size = client->readBytesUntil(' ', response_code, 10);
    response_code[size] = '\0';
    int code = atoi(response_code);
    free(response_code);

    #ifdef DEBUG_API
        Serial.printf("%d ", code);
    #endif
    
    while (client->connected()) {
        String line = client->readStringUntil('\n');

        #ifdef DEBUG_API
            Serial.println(line);
        #endif

        if (line == "\r") {
            break;
        }
    }

    return code;
}

String ApiDS::getResponseBody(Stream* stream) {
    String response;

    while(stream->available()){        
        response += stream->readStringUntil('\n'); 
    }      

    #ifdef DEBUG_API
        Serial.println(response); 
    #endif

    return response;
}

void ApiDS::printResponsePart(char* tag, char* buffer, size_t long_bytes) {
    if (long_bytes == 0) return;

    Serial.printf("%s", tag);

    for (unsigned int i = 0; i < long_bytes; i++) {
        Serial.print(buffer[i]);
    }

    Serial.print(F("\", "));
}

void ApiDS::printErrorMessage(int response_code) {
    Serial.print(F("Request failed. Response code: "));
    Serial.println(response_code);
}

String ApiDS::generateBodyRequest(String* request_body) {
    String request = "Content-Length: ";
    request += (*request_body).length();
    request += F("\r\n\r\n");
    request += *request_body;
    request += F("\r\n\r\n");

    return request;
}

unsigned int ApiDS::processResponse(char* tag, char* buffer, Stream* stream) {
    if (!stream->find(tag)) return -1;
    size_t size = stream->readBytesUntil('"', buffer, 30);

    #ifdef DEBUG_API
        printResponsePart(tag, buffer, size);
    #endif

    return size;
}

#endif