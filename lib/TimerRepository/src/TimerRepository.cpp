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
        projects = clockfy_ds->getProjects();
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

char* TimerRepository::startTimer(char* project_id) {
    char* date_time = datetime_ds->getDateTime();

    if (date_time != "") {
        char* timer_id = clockfy_ds->startTimer(project_id, date_time);
        return timer_id;
    } else {
        return "";
    }
}

void TimerRepository::stopTimer(char* timer_id) {
    
}
