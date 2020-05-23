
#include "unity.h"
#include "test_lamp.h"
#include "test_timer_repository.h"

#ifdef UNIT_TEST

int main(int argc, char **argv) {
    UNITY_BEGIN();
    run_lamp_tests();
    run_timer_repository_tests();
    UNITY_END();
}


#endif