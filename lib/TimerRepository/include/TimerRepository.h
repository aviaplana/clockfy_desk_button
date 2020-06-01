#ifndef TIMER_REPOSITORY_H
#define  TIMER_REPOSITORY_H

#include "Project.h"
#include "UserData.h"
#include "Clockfy/ClockfyDS.h"
#include "LocalDS.h"
#include "DateTime/DateTimeDS.h"

class TimerRepository {
    public:
        TimerRepository(LocalDS* local, ClockfyDS* clockfy_ds, DateTimeDS* datetime_ds);
        Project** getProjects();
        bool startTimer(char* project_id);
        bool stopTimer();
        Project* getProjectPositon(byte position);
        byte getNumProjects();
        char* getWorkspaceId();
        char* getUserId();


    private:
        LocalDS* local_ds;
        ClockfyDS* clockfy_ds;
        DateTimeDS* datetime_ds;

        bool loadUserData();
};

#endif