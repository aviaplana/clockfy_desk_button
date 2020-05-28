#include <string.h>
#include <stdlib.h>

#include "LocalDS.h"

LocalDS::LocalDS(ArduinoInterface* arduino_manager): arduino_manager(arduino_manager) {

}

void LocalDS::storeProjects(Project** projects) {
    this->projects = projects;
    last_updated = arduino_manager->doMillis();
}

bool LocalDS::dataExpired() {
    if (projects == NULL) {
        return true;
    }
    
    return ((arduino_manager->doMillis() - last_updated) > max_data_duration); 
}

Project** LocalDS::getProjects() {
    return projects;
}