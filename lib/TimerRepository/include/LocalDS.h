#ifndef LOCAL_DS_H
#define LOCAL_DS_H
#include <ArduinoInterface.h>
#include "DataSource.h"
#include "Project.h"

class LocalDS: public DataSource {
    public:
        LocalDS(ArduinoInterface* arduino_manager);
        Project** getProjects();
        void storeProjects(Project** projects);
        bool dataExpired();

    private:
        Project** projects = NULL;
        unsigned long last_updated = 0;
        const unsigned long max_data_duration = 600000; // 10 minutes
        ArduinoInterface* arduino_manager;
};

#endif