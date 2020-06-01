#ifndef CLOCKFY_DS_H
#define CLOCKFY_DS_H

#include "Project.h"
#include "UserData.h"

class ClockfyDS {
    public:
        virtual Project** getProjects(char* workspace_id) = 0;
        virtual UserData* getUserData() = 0;
        virtual bool startTimer(char* project_id, char* workspace_id, char* start_time) = 0;
        virtual bool stopTimer(char* stop_time, char* user_id, char* workspace_id) = 0;
};

#endif