//
// Created by kello on 21/04/24.
//

#include "unity.h"
#include "finder.h"

#define MAX_LEN 256

void setUp() {}

void tearDown() {}

test_find_YYYYDDMM() {
    char dest[MAX_LEN];
    int output = findDate(dest, "test20240421.pdf");
    TEST_ASSERT_EQUAL(output, DATE_FOUND);
    TEST_ASSERT_EQUAL(dest, "2024-04-21");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_find_YYYYDDMM);

    UNITY_END();

    return 0;
}