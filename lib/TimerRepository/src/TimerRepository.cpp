#include <stdlib.h>
#include "TimerRepository.h"

TimerRepository::TimerRepository(LocalDS* local, ClockfyDS* clockfy, DateTimeDS* datetime) {
    local_ds = local;
    clockfy_ds = clockfy;
    datetime_ds = datetime;
}

Project** TimerRepository::getProjects() {
    Project** projects = local_ds->getProjects();

    if (projects == NULL) {
        char* workspace_id = getWorkspaceId();

        if (workspace_id != NULL) {
            projects = clockfy_ds->getProjects(workspace_id);
            local_ds->storeProjects(projects);
        }
    }

    return projects;
}

Project* TimerRepository::getProjectPositon(byte position) {
    Project** projects = getProjects();

    return projects[position];
}

byte TimerRepository::getNumProjects() {
    byte count = 0;
    Project** projects = getProjects();
    
    while (projects[count] != NULL) {
        count++;
    }
    
    return count;
}

bool TimerRepository::loadUserData() {
    UserData* user_data = clockfy_ds->getUserData();
    
    if (user_data != NULL) {
        local_ds->storeUserData(user_data);
        return true;
    } else {
        return false;
    }
}

char* TimerRepository::getWorkspaceId() {
    char* workspace_id = local_ds->getWorkspaceId();

    if (workspace_id == NULL) {
        
        if (loadUserData()) {
            return local_ds->getWorkspaceId();
        }
        
        return NULL;
    }

    return workspace_id;
}

char* TimerRepository::getUserId() {
    char* user_id = local_ds->getUserId();

    if (user_id == NULL) {
        
        if (loadUserData()) {
            return local_ds->getUserId();
        }

        return NULL;
    }

    return user_id;
}

bool TimerRepository::startTimer(char* project_id) {
    char* date_time = datetime_ds->getDateTime();
    char* workspace_id = getWorkspaceId();

    if (date_time != "" && workspace_id != NULL) {
        return clockfy_ds->startTimer(project_id, workspace_id, date_time);
    } else {
        return false;
    }
}

bool TimerRepository::stopTimer() {
    char* date_time = datetime_ds->getDateTime();
    char* user_id = getUserId();
    char* workspace_id = getWorkspaceId();

    if (date_time != "" && user_id != NULL && workspace_id != NULL) {
        return clockfy_ds->stopTimer(date_time, user_id, workspace_id);
    } else {
        return false;
    }
}
