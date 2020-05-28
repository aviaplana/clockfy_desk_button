#ifndef TIMER_REPOSITORY_H
#define  TIMER_REPOSITORY_H

#include "Project.h"
#include "DataSource.h"
#include "LocalDS.h"

class TimerRepository {
    public:
        TimerRepository(LocalDS* local, DataSource* api);
        Project** getProjects();
        void startTimer(char* project_id);
        void stopTimer(char* project_id);

    private:
        LocalDS* local_ds;
        DataSource* api_ds;
};

#endif