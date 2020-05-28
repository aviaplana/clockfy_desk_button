#include "TimerRepository.h"
#include "unity.h"
#include <iostream>

#ifdef UNIT_TEST

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
};

class MockArduinoRepo: public ArduinoInterface {
    private:
        int getPinPosition(uint8_t pin) { return -1; }

    public:
        void doPinMode(uint8_t pin, uint8_t mode) { }

        void doDigitalWrite(uint8_t pin, uint8_t val) { }

        int doDigitalRead(uint8_t pin) { return -1; }

        uint8_t doDigitalPinToInterrupt(uint8_t pin) { return 1; }

        void doAttachInterrupt(uint8_t interruption_number, void (*isr)(), uint8_t mode) { };
    
        unsigned long doMillis() { return 0; }
};

MockArduinoRepo* arduino_mock = new MockArduinoRepo();
LocalDS* localDS = new LocalDS(arduino_mock);
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

    TEST_ASSERT_EQUAL_MESSAGE(num_api_projects, num_local_projects, "Projects are not saved to the LocalDS.");
}

void test_local_data_expiration() {
    TEST_MESSAGE("Starting get projects test...");

    TEST_ASSERT_FALSE_MESSAGE(localDS->dataExpired(), "Data has expired when it shouldn't.");

    Project** projects = repository.getProjects();

    TEST_ASSERT_TRUE_MESSAGE(localDS->dataExpired(), "Data didn't expired when it should.");
}

void run_timer_repository_tests() {
    RUN_TEST(test_api_data_should_be_stored_locally);
}

#endif