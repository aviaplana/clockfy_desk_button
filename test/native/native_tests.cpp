
#include "unity.h"
#include "test_lamp.h"
#include "test_timer_repository.h"
#include "test_button.h"

#ifdef UNIT_TEST

int main(int argc, char **argv) {
    UNITY_BEGIN();
    run_lamp_tests();
    run_timer_repository_tests();
    run_button_tests();
    UNITY_END();
}


#endif