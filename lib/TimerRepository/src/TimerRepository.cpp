#include <stdlib.h>
#include "TimerRepository.h"

TimerRepository::TimerRepository(LocalDS* local, DataSource* api) {
    local_ds = local;
    api_ds = api;
}

Project** TimerRepository::getProjects() {
    Project** projects;

    if (local_ds->dataExpired()) {
        projects = api_ds->getProjects();
        local_ds->storeProjects(projects);
    } else {
        projects = local_ds->getProjects();
    }
    
    return projects;
}

void TimerRepository::startTimer(char* project_id) {

}

void TimerRepository::stopTimer(char* project_id) {
    
}
