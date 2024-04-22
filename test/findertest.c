//
// Created by kello on 21/04/24.
//

#include "finder.h"
#include "stdio.h"
#include "string.h"

#define MAX_LEN 256

int test_number = 0;
int test_passed = 0;

void test_finder(int output, char* predicted, char* input);

int main(void) {
    test_finder(DATE_FOUND, "2024-04-22", "test20240422.pdf");
    test_finder(DATE_FOUND, "2024-04-22", "test240422test.pdf");
    test_finder(DATE_FOUND, "2022-04-24", "test24042022.pdf");

    printf("\n\nTest passed: %d/%d", test_passed, test_number);
    return 0;
}

void test_finder(int output, char* predicted, char* input) {
    test_number++;
    char dest[MAX_LEN];
    int passed = 0;
    int res = find_date(dest, input);

    if (res == output) passed++;
    if (strcmp(dest, predicted) == 0) passed++;

    printf("\ntest %d: ", test_number);
    if (passed == 2) {
        printf("passed\t\t");
        test_passed++;
    } else {
        printf("not passed\t");
    }
    printf("%d =?= %d", res, output);
    res < 0 || output < 0 ? printf("\t") : printf("\t\t");
    printf("%s =?= %s", dest, predicted);
}
