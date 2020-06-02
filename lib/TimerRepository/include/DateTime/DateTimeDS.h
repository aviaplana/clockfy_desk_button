#ifndef DATE_TIME_H
#define DATE_TIME_H

class DateTimeDS {
    public:
        virtual char* getDateTime() = 0;
        virtual void synchronize() = 0;
};

#endif