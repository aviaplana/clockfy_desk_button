#ifdef UNIT_TEST

#include "TimerRepository.h"
#include "../libs/fakeit/fakeit.hpp"
#include "unity.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

void test_api_data_should_be_stored_locally() {
    //Given
    fakeit::Mock<ClockfyDS> mock_clockfy;
    fakeit::Mock<DateTimeDS> mock_datetime;    
    LocalDS* local_ds = new LocalDS();
    TimerRepository repository = TimerRepository(local_ds, &mock_clockfy.get(), &mock_datetime.get());

    Project** projects_return = (Project**) malloc(sizeof(Project*));
    UserData user_data { "userid", "workspace" };
    projects_return[0] = (Project*) malloc(sizeof(Project));
    strcpy(projects_return[0]->id, "Id");
    strcpy(projects_return[0]->name, "Name");

    // When
    fakeit::When(Method(mock_clockfy, getProjects)).Return(projects_return);
    fakeit::When(Method(mock_clockfy, getUserData)).Return(&user_data);

    // Then
    TEST_ASSERT_TRUE_MESSAGE(local_ds->getProjects() == NULL, "LocalDS has projects.");
    
    Project** projects = repository.getProjects();
    unsigned int num_api_projects = 0;

    while(projects[num_api_projects] != NULL) {
        num_api_projects++;
    }

    projects = local_ds->getProjects();
    unsigned int num_local_projects = 0;

    while(projects[num_local_projects] != NULL) {
        num_local_projects++;
    }

    free(projects_return);

    TEST_ASSERT_EQUAL_MESSAGE(num_api_projects, num_local_projects, "Projects are not saved to the LocalDS.");
}

void run_timer_repository_tests() {
    RUN_TEST(test_api_data_should_be_stored_locally);
}

#endif