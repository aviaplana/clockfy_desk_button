#ifndef CLOCKFY_DS_H
#define CLOCKFY_DS_H

#include "Project.h"

class ClockfyDS {
    public:
        virtual Project** getProjects() = 0;
        virtual char* startTimer(char* project_id, char* start_time) = 0;
};

#endif