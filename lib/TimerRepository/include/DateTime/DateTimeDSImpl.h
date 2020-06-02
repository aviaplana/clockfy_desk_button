#if !defined(DATE_TIME_IMPL_H) && !defined(UNIT_TEST)
#define DATE_TIME_IMPL_H
#include "DateTimeDS.h"
#include "ApiDS.h"
#define DEBUG_API

class DateTimeDSImpl: public DateTimeDS, protected ApiDS {
    public:
        char* getDateTime();
        void synchronize();

    private:
        const char* ntp_server_1 = "pool.ntp.org";
        const char* ntp_server_2 = "time.nist.gov";
        const unsigned int timezone = 0;

        char* numberToStringLeadingZero(int number);
};

#endif