#ifndef API_DS_H
#define API_DS_H
#include "DataSource.h"

class ApiDS: public DataSource {
    public:
        ApiDS();
        Project** getProjects();

    private:
};

#endif