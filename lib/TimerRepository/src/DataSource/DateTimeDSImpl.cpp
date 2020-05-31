#ifndef UNIT_TEST

#include "DateTime/DateTimeDSImpl.h"

char* DateTimeDSImpl::getDateTime() {
    WiFiClient client;
    
    if (client.connect(host, port)) {       
        String endpoint = F("/api/timezone/");
        endpoint += TIME_ZONE;

        String request = F("GET ");
        request += endpoint;
        request += F(" HTTP/1.1\r\nHost: ");
        request += host;
        request += F("\r\nConnection: close\r\n\r\n");

        #ifdef DEBUG_API
            Serial.print(F("Sending request:\n"));
            Serial.print(request);
            Serial.print(F("\n"));
        #endif
        
        client.print(request);

        int response_code = readResponseHeaders(&client);

        if (response_code == 200) { 
            char* tag = "\"datetime\":\"";
            char* datetime = (char*) malloc(30);
            unsigned int size = processResponse(tag, datetime, (Stream*) &client);
            
            if (size > 0) {
                datetime[size] = '\0';
                return datetime;
            }
        } else {
            printErrorMessage(response_code);
        }
    }
    return "";
}


#endif