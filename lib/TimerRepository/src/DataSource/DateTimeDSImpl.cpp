#ifndef UNIT_TEST

#include "DateTime/DateTimeDSImpl.h"
#include "time.h"
#include "string.h"

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
}


#endif