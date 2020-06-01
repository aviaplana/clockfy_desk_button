#ifndef UNIT_TEST

#include "DateTime/DateTimeDSImpl.h"

char* DateTimeDSImpl::getDateTime() {
    WiFiClient client;
    
    if (client.connect(host, port)) {       
        String endpoint = F("/api/timezone/Etc/UTC");

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
            char* datetime = (char*) malloc(30);
            if (client.find("\"datetime\":\"")) {
                size_t size = client.readBytesUntil('+', datetime, 30);
                
                if (size > 0) {
                    datetime[size] = 'Z';
                    datetime[size+1] = '\0';
                    
                    #ifdef DEBUG_API
                        Serial.printf("Current datetime is %s.\n", datetime);
                    #endif

                    return datetime;
                }
            }

            #ifdef DEBUG_API
                Serial.println("Couldn't get response.");
            #endif
        } else {
            printErrorMessage(response_code);
        }
    }
    return "";
}


#endif