//
// Created by kello on 21/04/24.
//

#include "finder.h"
#include "stdio.h"
#include "string.h"

#define MAX_LEN 256

int test_number = 0;
int test_passed = 0;

void one_input_test(int argc, char **argv);

void no_input_test();

void spread_finder_input(int output, char *predicted, char *input);

void test_finder(int output, char *predicted, char *input);

void print_formatted(char *first, char *second, char *third, char *input);

int main(int argc, char **argv) {
    argc > 1 ?
    one_input_test(argc, argv) :
    no_input_test();
    return 0;
}

void one_input_test(int argc, char **argv) {
    char dest[MAX_LEN], temp[MAX_LEN];
    strcpy(temp, argv[1]);
    for (int i = 2; i < argc; i++) {
        strcat(temp, " ");
        strcat(temp, argv[i]);
    }
    int res = find_date(dest, temp);
    printf("|| %s -> %s (%d)", temp, dest, res);
}

void no_input_test() {
    test_finder(DATE_UNKNOWN, "0000-00-00", "void");
    test_finder(DATE_UNKNOWN, "0000-00-00", "void.png");
    test_finder(DATE_UNKNOWN, "0000-00-00", "void.tar.gz");

    spread_finder_input(DATE_FOUND, "2024-02-29", "29 feb 2024");
    spread_finder_input(DATE_UNKNOWN, "0000-00-00", "31 apr 2023");

    spread_finder_input(DATE_UNKNOWN, "0000-00-00", "12312");
    spread_finder_input(DATE_UNKNOWN, "0000-00-00", "1231212");
    spread_finder_input(DATE_UNKNOWN, "0000-00-00", "123");

    spread_finder_input(DATE_UNSURE, "2024-01-02", "4january2");
    spread_finder_input(DATE_UNSURE, "2024-04-22", "240422");

    spread_finder_input(DATE_UNSURE, "2024-04-22", "24 04 22");
    spread_finder_input(DATE_FOUND, "2024-04-02", "24 04 2");
    spread_finder_input(DATE_UNSURE, "2024-04-22", "24 4 22");
    spread_finder_input(DATE_FOUND, "2024-04-02", "24 4 2");

    spread_finder_input(DATE_FOUND, "2022-04-02", "2 04 22");
    spread_finder_input(DATE_FOUND, "2022-04-02", "2 4 22");

    spread_finder_input(DATE_UNSURE, "2024-04-22", "24-04-22");

    spread_finder_input(DATE_FOUND, "2032-04-22", "320422");
    spread_finder_input(DATE_FOUND, "2024-04-22", "22042024");

    spread_finder_input(DATE_FOUND, "2032-04-22", "22 04 32");
    spread_finder_input(DATE_FOUND, "2032-04-02", "2 04 32");
    spread_finder_input(DATE_FOUND, "2032-04-22", "22 4 32");
    spread_finder_input(DATE_FOUND, "2032-04-02", "2 4 32");

    spread_finder_input(DATE_FOUND, "2032-04-22", "22-04-32");
    spread_finder_input(DATE_FOUND, "2032-04-02", "2-04-32");
    spread_finder_input(DATE_FOUND, "2032-04-22", "22-4-32");
    spread_finder_input(DATE_FOUND, "2032-04-02", "2-4-32");

    spread_finder_input(DATE_FOUND, "2024-04-22", "22 04 2024");
    spread_finder_input(DATE_FOUND, "2024-04-02", "2 04 2024");
    spread_finder_input(DATE_FOUND, "2024-04-22", "22 4 2024");
    spread_finder_input(DATE_FOUND, "2024-04-02", "2 4 2024");

    spread_finder_input(DATE_FOUND, "2024-04-22", "22-04-2024");
    spread_finder_input(DATE_FOUND, "2024-04-02", "2-04-2024");
    spread_finder_input(DATE_FOUND, "2024-04-22", "22-4-2024");
    spread_finder_input(DATE_FOUND, "2024-04-02", "2-4-2024");

    spread_finder_input(DATE_FOUND, "2032-04-22", "220432");
    spread_finder_input(DATE_FOUND, "2024-04-22", "20240422");

    spread_finder_input(DATE_FOUND, "2032-04-22", "32 04 22");
    spread_finder_input(DATE_FOUND, "2032-04-02", "32 04 2");
    spread_finder_input(DATE_FOUND, "2032-04-22", "32 4 22");
    spread_finder_input(DATE_FOUND, "2032-04-02", "32 4 2");

    spread_finder_input(DATE_FOUND, "2032-04-22", "32-04-22");
    spread_finder_input(DATE_FOUND, "2032-04-02", "32-04-2");
    spread_finder_input(DATE_FOUND, "2032-04-22", "32-4-22");
    spread_finder_input(DATE_FOUND, "2032-04-02", "32-4-2");

    spread_finder_input(DATE_FOUND, "2024-04-22", "2024 04 22");
    spread_finder_input(DATE_FOUND, "2024-04-02", "2024 04 2");
    spread_finder_input(DATE_FOUND, "2024-04-22", "2024 4 22");
    spread_finder_input(DATE_FOUND, "2024-04-02", "2024 4 2");

    spread_finder_input(DATE_FOUND, "2024-04-22", "2024-04-22");
    spread_finder_input(DATE_FOUND, "2024-04-02", "2024-04-2");
    spread_finder_input(DATE_FOUND, "2024-04-22", "2024-4-22");
    spread_finder_input(DATE_FOUND, "2024-04-02", "2024-4-2");

    spread_finder_input(DATE_FOUND, "2024-04-22", "0422");
    spread_finder_input(DATE_FOUND, "2024-04-22", "2204");

    spread_finder_input(DATE_FOUND, "2024-04-22", "04 22");
    spread_finder_input(DATE_FOUND, "2024-04-22", "4 22");
    spread_finder_input(DATE_UNSURE, "2024-04-02", "4 2");

    spread_finder_input(DATE_FOUND, "2024-04-22", "04-22");
    spread_finder_input(DATE_FOUND, "2024-04-22", "4-22");
    spread_finder_input(DATE_UNSURE, "2024-04-02", "4-2");

    spread_finder_input(DATE_FOUND, "2024-04-22", "22 04");
    spread_finder_input(DATE_FOUND, "2024-04-22", "22 4");
    spread_finder_input(DATE_FOUND, "2024-04-22", "22-04");
    spread_finder_input(DATE_FOUND, "2024-04-22", "22-4");

    // Now literal months

    spread_finder_input(DATE_UNSURE, "2024-01-22", "24january22");
    spread_finder_input(DATE_FOUND, "2022-01-02", "2january22");
    spread_finder_input(DATE_FOUND, "2024-01-02", "24january2");

    spread_finder_input(DATE_FOUND, "2024-02-22", "22feb2024");
    spread_finder_input(DATE_FOUND, "2024-02-02", "2feb2024");

    spread_finder_input(DATE_UNSURE, "2022-03-24", "22 Marzo 24");
    spread_finder_input(DATE_FOUND, "2032-03-22", "22 Marzo 32");
    spread_finder_input(DATE_FOUND, "2024-03-02", "2 Marzo 24");

    spread_finder_input(DATE_FOUND, "2032-04-22", "22-apr-32");
    spread_finder_input(DATE_FOUND, "2032-04-02", "2-apr-32");
    spread_finder_input(DATE_FOUND, "2024-05-22", "22 may 2024");
    spread_finder_input(DATE_FOUND, "2024-05-02", "2 may 2024");
    spread_finder_input(DATE_FOUND, "2024-06-22", "22-JUN-2024");
    spread_finder_input(DATE_FOUND, "2024-06-02", "2-JUN-2024");

    spread_finder_input(DATE_FOUND, "2032-07-22", "22luglio32");
    spread_finder_input(DATE_FOUND, "2032-07-02", "2luglio32");
    spread_finder_input(DATE_FOUND, "2024-08-22", "2024ago22");
    spread_finder_input(DATE_FOUND, "2024-08-02", "2024ago2");
    spread_finder_input(DATE_FOUND, "2032-09-22", "32 September 22");
    spread_finder_input(DATE_FOUND, "2032-09-02", "32 September 2");
    spread_finder_input(DATE_FOUND, "2032-10-22", "32-oct-22");
    spread_finder_input(DATE_FOUND, "2032-10-02", "32-oct-2");
    spread_finder_input(DATE_FOUND, "2024-11-22", "2024 novembre 22");
    spread_finder_input(DATE_FOUND, "2024-11-02", "2024 novembre 2");
    spread_finder_input(DATE_FOUND, "2024-12-22", "2024-DIC-22");
    spread_finder_input(DATE_FOUND, "2024-12-02", "2024-DIC-2");

    spread_finder_input(DATE_FOUND, "2024-01-22", "gen22");
    spread_finder_input(DATE_FOUND, "2024-01-02", "gen2");
    spread_finder_input(DATE_FOUND, "2024-02-22", "22febbraio");
    spread_finder_input(DATE_FOUND, "2024-02-02", "2febbraio");
    spread_finder_input(DATE_FOUND, "2024-03-22", "Mar 22");
    spread_finder_input(DATE_FOUND, "2024-03-02", "Mar 2");
    spread_finder_input(DATE_FOUND, "2024-04-22", "april-22");
    spread_finder_input(DATE_FOUND, "2024-04-02", "april-2");
    spread_finder_input(DATE_FOUND, "2024-05-22", "22 MAG");
    spread_finder_input(DATE_FOUND, "2024-05-02", "2 MAG");
    spread_finder_input(DATE_FOUND, "2024-06-22", "22-giugno");
    spread_finder_input(DATE_FOUND, "2024-06-02", "2-giugno");

    // Not this year (2024)

    spread_finder_input(DATE_FOUND, "2025-11-22", "2025 novembre 22");
    spread_finder_input(DATE_FOUND, "2025-11-02", "2025 novembre 2");
    spread_finder_input(DATE_FOUND, "2025-12-22", "2025-DIC-22");
    spread_finder_input(DATE_FOUND, "2025-12-02", "2025-DIC-2");
    spread_finder_input(DATE_FOUND, "2025-08-22", "2025ago22");
    spread_finder_input(DATE_FOUND, "2025-08-02", "2025ago2");
    spread_finder_input(DATE_FOUND, "2025-05-22", "22 may 2025");
    spread_finder_input(DATE_FOUND, "2025-05-02", "2 may 2025");
    spread_finder_input(DATE_FOUND, "2025-06-22", "22-JUN-2025");
    spread_finder_input(DATE_FOUND, "2025-06-02", "2-JUN-2025");
    spread_finder_input(DATE_FOUND, "2025-02-22", "22feb2025");
    spread_finder_input(DATE_FOUND, "2025-02-02", "2feb2025");
    spread_finder_input(DATE_FOUND, "2025-04-22", "2025 04 22");
    spread_finder_input(DATE_FOUND, "2025-04-02", "2025 04 2");
    spread_finder_input(DATE_FOUND, "2025-04-22", "2025 4 22");
    spread_finder_input(DATE_FOUND, "2025-04-02", "2025 4 2");
    spread_finder_input(DATE_FOUND, "2025-04-22", "2025-04-22");
    spread_finder_input(DATE_FOUND, "2025-04-02", "2025-04-2");
    spread_finder_input(DATE_FOUND, "2025-04-22", "2025-4-22");
    spread_finder_input(DATE_FOUND, "2025-04-02", "2025-4-2");
    spread_finder_input(DATE_FOUND, "2025-04-22", "20250422");
    spread_finder_input(DATE_FOUND, "2025-04-22", "22 04 2025");
    spread_finder_input(DATE_FOUND, "2025-04-02", "2 04 2025");
    spread_finder_input(DATE_FOUND, "2025-04-22", "22 4 2025");
    spread_finder_input(DATE_FOUND, "2025-04-02", "2 4 2025");
    spread_finder_input(DATE_FOUND, "2025-04-22", "22-04-2025");
    spread_finder_input(DATE_FOUND, "2025-04-02", "2-04-2025");
    spread_finder_input(DATE_FOUND, "2025-04-22", "22-4-2025");
    spread_finder_input(DATE_FOUND, "2025-04-02", "2-4-2025");
    spread_finder_input(DATE_FOUND, "2025-04-22", "22042025");
    spread_finder_input(DATE_UNSURE, "2025-04-22", "25-04-22");
    spread_finder_input(DATE_UNSURE, "2025-04-22", "250422");
    spread_finder_input(DATE_UNSURE, "2025-04-22", "25 04 22");
    spread_finder_input(DATE_FOUND, "2025-04-02", "25 04 2");
    spread_finder_input(DATE_UNSURE, "2025-04-22", "25 4 22");
    spread_finder_input(DATE_FOUND, "2025-04-02", "25 4 2");

    printf("\nTest passed: %d/%d", test_passed, test_number);
}

void spread_finder_input(int output, char *predicted, char *input) {
    char temp[MAX_LEN];
    test_finder(output, predicted, input);

    sprintf(temp, "some%s", input);
    test_finder(output, predicted, temp);
    sprintf(temp, "%sthing", input);
    test_finder(output, predicted, temp);
    sprintf(temp, "some%sthing.tmp", input);
    test_finder(output, predicted, temp);
}

void test_finder(int output, char *predicted, char *input) {
#define TN_LEN 3
    test_number++;
    char dest[MAX_LEN], first[MAX_LEN], second[MAX_LEN], third[MAX_LEN];
    int passed = 0;
    int res = find_date(dest, input);
    if (res == output) passed++;
    if (strcmp(dest, predicted) == 0) passed++;
    if (passed == 2) {
        sprintf(first, "test %.*d: passed", TN_LEN, test_number);
        test_passed++;
    } else
        sprintf(first, "test %.*d: not passed", TN_LEN, test_number);
    sprintf(second, "%2d =?= %d", res, output);
    sprintf(third, "%.125s =?= %.125s", dest, predicted);
    print_formatted(first, second, third, input);
#undef TN_LEN
}

void print_formatted(char *first, char *second, char *third, char *input) {
#define PASS_LEN 24
#define OUT_LEN 16
#define COMP_LEN 32
    printf("%s", first);
    for (int i = 0; i < PASS_LEN - strlen(first); i++) printf(" ");
    printf("%s", second);
    for (int i = 0; i < OUT_LEN - strlen(second); i++) printf(" ");
    printf("%s", third);
    for (int i = 0; i < COMP_LEN - strlen(third); i++) printf(" ");
    printf("%s\n", input);
#undef COMP_LEN
#undef PASS_LEN
#undef OUT_LEN
}
