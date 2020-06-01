#if !defined(DATE_TIME_IMPL_H) && !defined(UNIT_TEST)
#define DATE_TIME_IMPL_H
#include <WiFiClient.h> 
#include "DateTimeDS.h"
#include "ApiDs.h"
#define DEBUG_API

class DateTimeDSImpl: public DateTimeDS, protected ApiDS {
    public:
        char* getDateTime();

    private:
        const char* host = "worldtimeapi.org";
        const int port = 80;
};

#endif