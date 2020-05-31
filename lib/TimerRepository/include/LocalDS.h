#ifndef LOCAL_DS_H
#define LOCAL_DS_H
#include "Project.h"

class LocalDS {
    public:
        LocalDS();
        Project** getProjects();
        void storeProjects(Project** projects);

    private:
        Project** projects = NULL;
};

#endif