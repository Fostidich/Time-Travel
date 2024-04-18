//
// Created by kello on 04/04/24.
//

#include <regex.h>
#include <stdlib.h>
#include "finder.h"
#include "main.h"

int findDate(char *dest, const char *filename) {
    int nums[3] = {0};

    char *pattern = "((\\d+)(.|-|/|\\)){2,3}";
    regex_t regex;
    int reg_out = regcomp(&regex, pattern, REG_EXTENDED);
    if (reg_out) return DATE_UNKNOWN;

    regmatch_t matches[3];
    reg_out = regexec(&regex, filename, 3, matches, 0);

    if (!reg_out) {
        int nums_index = 0;
        for (int i = 1; i < 4 && matches[i].rm_so != -1; i++) {
            char *match_str = &filename[matches[i].rm_so];
            int len = matches[i].rm_eo - matches[i].rm_so;

            int num = strtol(match_str, NULL, 10);

            nums[nums_index++] = num;
        }

        regfree(&regex);
    }
}