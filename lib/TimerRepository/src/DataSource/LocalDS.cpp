#include <string.h>
#include <stdlib.h>

#include "LocalDS.h"

LocalDS::LocalDS() {

}

void LocalDS::storeProjects(Project** projects) {
    this->projects = projects;
}

void LocalDS::storeUserData(UserData* user_data) {
    this->user_data = user_data;
}

char* LocalDS::getUserId() {
    if (this->user_data != NULL) {
        return this->user_data->user_id;
    } else {
        return NULL;
    }
}

char* LocalDS::getWorkspaceId() {
    if (this->user_data != NULL) {
        return this->user_data->workspace_id;
    } else {
        return NULL;
    }
}

Project** LocalDS::getProjects() {
    return projects;
}