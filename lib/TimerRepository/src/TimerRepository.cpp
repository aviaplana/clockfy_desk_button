#include <stdlib.h>
#include "TimerRepository.h"

TimerRepository::TimerRepository() {
    local_ds = new LocalDS();
    api_ds = new ApiDS();
}

TimerRepository::TimerRepository(DataSource* local, DataSource* api) {
    local_ds = local;
    api_ds = api;
}

Project** TimerRepository::getProjects() {
    return api_ds->getProjects();
}


void TimerRepository::startTimer(char* project_id) {

}

void TimerRepository::stopTimer(char* project_id) {
    
}
