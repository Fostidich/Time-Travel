#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "finder.h"

#define BUF_LEN 256

int (*finder)(char *, const char *) = find_date;

int test_line(const char *line);
int test_check(int outcome, const char *expected, const char *source);
int extract_from_line(int *outcome, char *expected, char *source, const char *buffer);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "No file path provided");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open the provided file");
        return 1;
    }

    char buffer[BUF_LEN];
    int count = 0;
    int fails = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++;
        switch (test_line(buffer)) {
            case -1:
                fprintf(stderr, "\033[33m!\033[0m %d: unable to read line", count);
                fails++;
                continue;
            case 0:
                printf("\033[31mx\033[0m %d: %s", count, buffer);
                fails++;
                continue;
            case 1:
                printf("\033[32mv\033[0m %d: %s", count, buffer);
                continue;
        }
    }

    fclose(file);
    return fails;
}

int test_line(const char *line) {
    int outcome;
    char expected[BUF_LEN];
    char source[BUF_LEN];
    if (!extract_from_line(&outcome, expected, source, line)) return -1;
    return test_check(outcome, expected, source);
}

int test_check(int outcome, const char *expected, const char *source) {
    char result[BUF_LEN];
    int state = finder(result, source);
    return strcmp(expected, result) == 0 && outcome == state;
}

int extract_from_line(int *outcome, char *expected, char *source, const char *buffer) {
    int i = 0, j = 0;

    while (buffer[i] == ' ') i++;
    switch (buffer[i++]) {
        case '0':
            *outcome = 0;
            break;
        case '1':
            *outcome = 1;
            break;
        case '2':
            *outcome = 2;
            break;
        case '3':
            *outcome = 3;
            break;
        case '4':
            *outcome = 4;
            break;
        default:
            return -1;
    }

    if (buffer[i++] != ' ') return 0;
    while (buffer[i] == ' ') i++;
    while (buffer[i] != ' ') expected[j++] = buffer[i++];
    expected[j] = '\0';
    j = 0;
    i++;

    while (buffer[i] == ' ') i++;
    while (buffer[i] != ' ' && buffer[i] != '\n') source[j++] = buffer[i++];
    source[j] = '\0';
    return 1;
}
