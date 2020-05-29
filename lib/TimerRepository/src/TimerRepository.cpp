#include <stdlib.h>
#include "TimerRepository.h"

TimerRepository::TimerRepository(LocalDS* local, DataSource* api) {
    local_ds = local;
    api_ds = api;
}

Project** TimerRepository::getProjects() {
    Project** projects = local_ds->getProjects();

    if (projects == NULL) {
        projects = api_ds->getProjects();
        local_ds->storeProjects(projects);
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

void TimerRepository::startTimer(char* project_id) {

}

void TimerRepository::stopTimer(char* project_id) {
    
}
