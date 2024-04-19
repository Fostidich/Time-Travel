//
// Created by kello on 04/04/24.
//

#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include "finder.h"
#include "main.h"

// TODO: remember to find months in word format

#define CAPTURES 3

int findDate(char *dest, const char *filename) {
    const char *pattern = "([[:digit:]]{2})[-| |\\]([[:digit:]]{2})[-| |\\]([[:digit:]]{2})";
    regex_t regex;
    regmatch_t matches[CAPTURES + 1];
    int nums[CAPTURES] = {0};

    // regex compilation providing pattern
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) return DATE_UNKNOWN;

    // regex execution for pattern matching
    if (regexec(&regex, filename, CAPTURES + 1, matches, 0) != 0) return DATE_UNKNOWN;


    for (int g = 0; g < CAPTURES; g++) {
        if (matches[g + 1].rm_so == -1) return DATE_UNKNOWN;
        char temp[12];
        sprintf(temp, "%.*s", matches[g + 1].rm_eo - matches[g + 1].rm_so, filename + matches[g + 1].rm_so);
        nums[g] = atoi(temp); // NOLINT(*-err34-c)
    }

    sprintf(dest, "20%02d-%02d-%02d", nums[2], nums[1], nums[0]);
    regfree(&regex);

    return DATE_FOUND;
}

/*
 * FORMATS:
 * [YYYY|YY]MMDD
 * [YYYY|YY][\| |-][MM|M][\| |-][DD|D]
 * DDMM[YYYY|YY]
 * [DD|D][\| |-][MM|M][\| |-][YYYY|YY]
 * DDMM
 * [DD|D][\| |-][MM|M]
 * MMDD
 * [MM|M][\| |-][DD|D]
 * *months with alpha chars
 */
