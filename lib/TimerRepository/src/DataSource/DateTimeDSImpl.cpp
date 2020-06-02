#ifndef UNIT_TEST

#include "DateTime/DateTimeDSImpl.h"
#include "time.h"

void DateTimeDSImpl::synchronize() {
    #ifdef DEBUG_API
        Serial.print("Setting time using SNTP");
    #endif

    configTime(timezone, 0, ntp_server_1, ntp_server_2);
    time_t now = time(nullptr);

    while (now < 1000) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    
    #ifdef DEBUG_API
        Serial.print("Current time: ");
        Serial.print(asctime(&timeinfo));
    #endif
}

char* DateTimeDSImpl::getDateTime() {
    struct tm *timeinfo;
    time_t now;

    time(&now);
    timeinfo = localtime(&now);
    
    int year = timeinfo->tm_year + 1900;

    char* date = (char*) malloc(21);
    sprintf(date, "%d-%02d-%02dT%02d:%02d:%02dZ", 
        year, 
        timeinfo->tm_mon + 1, 
        timeinfo->tm_mday, 
        timeinfo->tm_hour, 
        timeinfo->tm_min, 
        timeinfo->tm_sec);

    #ifdef DEBUG_API
        Serial.printf("DateTime: %s\n", date);
    #endif

    return date;

//2020-06-02T16:21:10.672607Z
/*
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
    */
}


#endif