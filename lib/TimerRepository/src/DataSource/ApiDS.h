#ifndef API_DS_H
#define API_DS_H

#include <WiFiClientSecure.h> 
#include "DataSource.h"
#include "api_sensible_data.h"

class ApiDS: public DataSource {
    public:
        ApiDS();
        Project** getProjects();

    private:
        BearSSL::WiFiClientSecure getConnectedClient();
        const char* host = "api.clockify.me";
        const int https_port = 443;
        const unsigned int max_retry = 10;
        const unsigned long timeout = 15000;
        const char* fingerprint = "4C 70 A3 F9 41 36 E3 03 99 0A 27 57 52 DA 45 D6 42 8E E4 2B";
};

#endif