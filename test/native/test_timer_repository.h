#ifdef UNIT_TEST

#include "TimerRepository.h"
#include "unity.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

#define NUM_PROJECTS 4

class MockApiDS: public DataSource {
    public:
        MockApiDS() {}

        Project** getProjects() {
            Project** projects = (Project**) malloc(sizeof(Project*));
            projects[0] = (Project*) malloc(sizeof(Project));
            strcpy(projects[0]->id, "Id");
            strcpy(projects[0]->name, "Name");
            
            return projects;
        }
};LocalDS* localDS = new LocalDS();
MockApiDS* apiDS = new MockApiDS();

TimerRepository repository = TimerRepository(localDS, apiDS);

void test_api_data_should_be_stored_locally() {
    TEST_MESSAGE("Starting get projects test...");
    TEST_ASSERT_TRUE_MESSAGE(localDS->getProjects() == NULL, "LocalDS has projects.");

    Project** projects = repository.getProjects();

    unsigned int num_api_projects = 0;

    while(projects[num_api_projects] != NULL) {
        num_api_projects++;
    }

    projects = localDS->getProjects();

    unsigned int num_local_projects = 0;

    while(projects[num_local_projects] != NULL) {
        num_local_projects++;
    }

    free(projects);

    TEST_ASSERT_EQUAL_MESSAGE(num_api_projects, num_local_projects, "Projects are not saved to the LocalDS.");
}

void run_timer_repository_tests() {
    RUN_TEST(test_api_data_should_be_stored_locally);
}

#endif