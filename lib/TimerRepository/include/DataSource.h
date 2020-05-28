#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H
#include "Project.h"

class DataSource {
    public:
        virtual Project** getProjects() = 0;
};

#endif