#ifndef TIMER_REPOSITORY_H
#define  TIMER_REPOSITORY_H

#include "entity/Project.h"
#include "DataSource/DataSource.h"
#include "DataSource/ApiDS.h"
#include "DataSource/LocalDS.h"

class TimerRepository {
    public:
        TimerRepository();
        TimerRepository(DataSource* local, DataSource* api); // For testing pourposes
        Project** getProjects();
        void startTimer(char* project_id);
        void stopTimer(char* project_id);

    private:
        DataSource* local_ds;
        DataSource* api_ds;
};

#endif