#ifndef LOCAL_DS_H
#define LOCAL_DS_H
#include "DataSource.h"

class LocalDS: public DataSource {
    public:
        LocalDS();
        Project** getProjects();

    private:
};

#endif