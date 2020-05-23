#include <string.h>
#include <stdlib.h>

#include "LocalDS.h"

LocalDS::LocalDS() {

}

Project** LocalDS::getProjects() {
    const unsigned int num_projects = 4;
    struct Project** projects = (struct Project**) malloc(sizeof(struct Project*) * num_projects);

    for (int i = 0; i < num_projects; i++) {
        char project_num = i + '0';
        projects[i] = (struct Project*) malloc(sizeof(struct Project));

        strcpy(projects[i]->id, strcat("FakeId", &project_num));
        strcpy(projects[i]->name, strcat("Project name ", &project_num));
        strcpy(projects[i]->color, "#AABBCC");
    }

    return projects;
}