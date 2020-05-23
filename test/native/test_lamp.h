#include "Lamp.h"
#include "utils/ArduinoMock.cpp"
#include "unity.h"

#ifdef UNIT_TEST

#define RED_LED_PIN 1
#define GREEN_LED_PIN 2
#define BLUE_LED_PIN 3

MockArduino mockArduino = MockArduino();
Lamp lamp = Lamp(&mockArduino, RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN);

void test_red(void) {
    // Given
    Color red = {1, 0, 0};

    // When
    lamp.change_color(red);

    // Then
    TEST_ASSERT_EQUAL(HIGH, mockArduino.doDigitalRead(RED_LED_PIN));
    TEST_ASSERT_EQUAL(LOW, mockArduino.doDigitalRead(GREEN_LED_PIN));
    TEST_ASSERT_EQUAL(LOW, mockArduino.doDigitalRead(BLUE_LED_PIN));
}

void test_blue(void) {
    // Given
    Color blue = {0, 0, 1};

    // When
    lamp.change_color(blue);

    // Then
    TEST_ASSERT_EQUAL(LOW, mockArduino.doDigitalRead(RED_LED_PIN));
    TEST_ASSERT_EQUAL(LOW, mockArduino.doDigitalRead(GREEN_LED_PIN));
    TEST_ASSERT_EQUAL(HIGH, mockArduino.doDigitalRead(BLUE_LED_PIN));
}

void test_green(void) {
    // Given
    Color green = {0, 1, 0};

    // When
    lamp.change_color(green);

    // Then
    TEST_ASSERT_EQUAL(LOW, mockArduino.doDigitalRead(RED_LED_PIN));
    TEST_ASSERT_EQUAL(HIGH, mockArduino.doDigitalRead(GREEN_LED_PIN));
    TEST_ASSERT_EQUAL(LOW, mockArduino.doDigitalRead(BLUE_LED_PIN));

}

void run_lamp_tests() {
    lamp.setup();
    RUN_TEST(test_red);
}

#endif