#ifdef UNIT_TEST


#include <ArduinoInterface.h>
#include "../libs/fakeit/fakeit.hpp"
#include "Button.h"
#include "unity.h"

#define BUTTON_PIN 1

void test_no_press(void) {
    // Given
    fakeit::Mock<ArduinoInterface> mock;
    Button button = Button(&mock.get(), BUTTON_PIN);

    // When
    fakeit::When(Method(mock, doMillis)).Return(100);
    fakeit::When(Method(mock, doDigitalRead)).Return(LOW);

    // Then
    TEST_ASSERT_EQUAL_MESSAGE(NO_PRESS, button.wasPressed(), "Should be NO_PRESS");
}

void test_short_press(void) {
    // Given
    unsigned long initial_millis = 100;
    fakeit::Mock<ArduinoInterface> mock;
    Button button = Button(&mock.get(), BUTTON_PIN);
    fakeit::When(Method(mock, doMillis)).Return(initial_millis);
    fakeit::When(Method(mock, doDigitalRead)).Return(LOW);
    button.wasPressed();

    // When
    fakeit::When(Method(mock, doMillis)).Return(initial_millis + 100);
    fakeit::When(Method(mock, doDigitalRead)).Return(HIGH);

    // Then
    TEST_ASSERT_EQUAL_MESSAGE(SHORT_PRESS, button.wasPressed(), "Should be SHORT_PRESS");
}

void test_long_press(void) {
    // Given
    unsigned long initial_millis = 100;
    fakeit::Mock<ArduinoInterface> mock;
    Button button = Button(&mock.get(), BUTTON_PIN);
    fakeit::When(Method(mock, doMillis)).Return(initial_millis);
    fakeit::When(Method(mock, doDigitalRead)).Return(LOW);
    button.wasPressed();

    // When
    fakeit::When(Method(mock, doMillis)).Return(initial_millis + 600);
    fakeit::When(Method(mock, doDigitalRead)).Return(HIGH);

    // Then
    TEST_ASSERT_EQUAL_MESSAGE(LONG_PRESS, button.wasPressed(), "Should be LONG_PRESS");
}

void run_button_tests() {
    RUN_TEST(test_no_press);
    RUN_TEST(test_short_press);
    RUN_TEST(test_long_press);
}

#endif
