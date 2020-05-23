#include "TimerRepository.h"
#include "unity.h"
#include <iostream>

#ifdef UNIT_TEST

#define NUM_PROJECTS 4

class MockApiDS: public DataSource {
    public:
        MockApiDS() {}

        Project** getProjects() {
            return 0;
        }
};

class MockLocalDS: public DataSource {
    public:
        MockLocalDS() {}

        Project** getProjects() {
            struct Project** projects = (struct Project**) malloc(sizeof(struct Project*) * NUM_PROJECTS);

            for (int i = 0; i < NUM_PROJECTS; i++) {
                projects[i] = (struct Project*) malloc(sizeof(struct Project));
                
                strcpy(projects[i]->id, "FakeId");
                strcpy(projects[i]->name, "Project name");
                strcpy(projects[i]->color, "#AABBCC");
            }

            return projects;
        }
};

MockLocalDS* localDS = new MockLocalDS();
MockApiDS* apiDS = new MockApiDS();

TimerRepository repository = TimerRepository(localDS, apiDS);

void test_get_projects() {
    TEST_MESSAGE("Starting get projects test...");
    Project** projects = repository.getProjects();
    
    for (int i = 0; i < NUM_PROJECTS; i++) {
        TEST_ASSERT_NOT_NULL_MESSAGE(projects[i], "Project not generated");
        TEST_ASSERT_EQUAL_STRING_MESSAGE("#AABBCC", projects[i]->color, "Wrong colo");  
        TEST_ASSERT_EQUAL_STRING_MESSAGE( "Project name", projects[i]->name, "Failed!!!");  
        TEST_ASSERT_EQUAL_STRING_MESSAGE("FakeId", projects[i]->id, "Failed!!!");  
    }
}

void run_timer_repository_tests() {
    RUN_TEST(test_get_projects);
}

#endif