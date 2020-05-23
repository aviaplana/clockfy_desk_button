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
    return local_ds->getProjects();
}
