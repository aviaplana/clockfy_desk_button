#ifndef TIMER_REPOSITORY_H
#define  TIMER_REPOSITORY_H

#include "Project.h"
#include "Clockfy/ClockfyDS.h"
#include "LocalDS.h"
#include "DateTime/DateTimeDS.h"

class TimerRepository {
    public:
        TimerRepository(LocalDS* local, ClockfyDS* clockfy_ds, DateTimeDS* datetime_ds);
        Project** getProjects();
        void startTimer(char* project_id);
        void stopTimer(char* project_id);
        Project* getProjectPositon(byte position);
        byte getNumProjects();

    private:
        LocalDS* local_ds;
        ClockfyDS* clockfy_ds;
        DateTimeDS* datetime_ds;
};

#endif