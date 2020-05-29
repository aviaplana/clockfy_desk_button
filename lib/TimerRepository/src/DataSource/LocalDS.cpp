#include <string.h>
#include <stdlib.h>

#include "LocalDS.h"

LocalDS::LocalDS() {

}

void LocalDS::storeProjects(Project** projects) {
    this->projects = projects;
}

Project** LocalDS::getProjects() {
    return projects;
}