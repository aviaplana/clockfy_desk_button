#ifndef LOCAL_DS_H
#define LOCAL_DS_H
#include "DataSource.h"
#include "Project.h"

class LocalDS: public DataSource {
    public:
        LocalDS();
        Project** getProjects();
        void storeProjects(Project** projects);

    private:
        Project** projects = NULL;
};

#endif