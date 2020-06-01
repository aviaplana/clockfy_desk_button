#ifndef LOCAL_DS_H
#define LOCAL_DS_H
#include "Project.h"
#include "UserData.h"

class LocalDS {
    public:
        LocalDS();
        Project** getProjects();
        char* getUserId();
        char* getWorkspaceId();
        void storeProjects(Project** projects);
        void storeUserData(UserData* user_data);

    private:
        Project** projects = NULL;
        UserData* user_data = NULL;
};

#endif