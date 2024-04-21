//
// Created by kello on 04/04/24.
//

#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include "finder.h"
#include "main.h"

// TODO: remember to find months in word format
// TODO: check for multiple matches

const char *months[4][12] = {
        {"january", "february", "march", "april",  "may",    "june",   "july",   "august", "september", "october", "november", "december"},
        {"jan",     "feb",      "mar",   "apr",    "may",    "jun",    "jul",    "aug",    "sep",       "oct",     "nov",      "dec"},
        {"gennaio", "febbraio", "marzo", "aprile", "maggio", "giugno", "luglio", "agosto", "settembre", "ottobre", "novembre", "dicembre"},
        {"gen",     "feb",      "mar",   "apr",    "mag",    "giu",    "lug",    "ago",    "set",       "ott",     "nov",      "dic"}
};

int findDate(char *dest, const char *filename) {
    int found = 0;
    regex_t *regex = malloc(sizeof(regex_t));
    regcomp(regex, "", 0);
    date_t *date = calloc(1, sizeof(date_t));

    // searches must be put from less likely to most likely

    found += searchDMY(regex, "([[:digit:]]{2})([[:digit:]]{2})([[:digit:]]{2})",
                       filename, date); // 210424
    found += searchDMY(regex, "([[:digit:]]{2})([[:digit:]]{2})20([[:digit:]]{2})",
                       filename, date); // 21042024
    found += searchDMY(regex, "([[:digit:]]{1,2}) ([[:digit:]]{1,2}) ([[:digit:]]{2})[[^[:digit:]]|$]",
                       filename, date); // 21 04 24
    found += searchDMY(regex, "([[:digit:]]{1,2}) ([[:digit:]]{1,2}) 20([[:digit:]]{2})",
                       filename, date); // 21 04 2024
    found += searchDMY(regex, "([[:digit:]]{1,2})\\([[:digit:]]{1,2})\\([[:digit:]]{2})[[^[:digit:]]|$]",
                       filename, date); // 21\04\24
    found += searchDMY(regex, "([[:digit:]]{1,2})\\([[:digit:]]{1,2})\\20([[:digit:]]{2})",
                       filename, date); // 21\04\2024
    found += searchDMY(regex, "([[:digit:]]{1,2})-([[:digit:]]{1,2})-([[:digit:]]{2})[[^[:digit:]]|$]",
                       filename, date); // 21-04-24
    found += searchDMY(regex, "([[:digit:]]{1,2})-([[:digit:]]{1,2})-20([[:digit:]]{2})",
                       filename, date); // 21-04-2024

    found += searchYMD(regex, "([[:digit:]]{2})([[:digit:]]{2})([[:digit:]]{2})[[^[:digit:]]|$]",
                       filename, date); // 240421
    found += searchYMD(regex, "20([[:digit:]]{2})([[:digit:]]{2})([[:digit:]]{2})",
                       filename, date); // 20240421
    found += searchYMD(regex, "([[:digit:]]{2}) ([[:digit:]]{1,2}) ([[:digit:]]{1,2})",
                       filename, date); // 2024 04 21 // 24 04 21
    found += searchYMD(regex, "([[:digit:]]{2})\\([[:digit:]]{1,2})\\([[:digit:]]{1,2})",
                       filename, date); // 2024\04\21 // 24\04\21
    found += searchYMD(regex, "([[:digit:]]{2})-([[:digit:]]{1,2})-([[:digit:]]{1,2})",
                       filename, date); // 2024-04-21 // 24-04-21

    if (found > 0) goto FINISH;

    found += searchMD(regex, "([[:digit:]]{2})([[:digit:]]{2})",
                      filename, date);

    found += searchDM(regex, "([[:digit:]]{2})([[:digit:]]{2})",
                      filename, date);

FINISH:

    sprintf(dest, "%d-%02d-%02d", date->year, date->month, date->day);
    regfree(regex);
    free(regex);
    free(date);

    switch (found) {
        case 1:
            return DATE_FOUND;
        case 0:
            return DATE_UNKNOWN;
        default:
            return DATE_UNSURE;
    }
}

int searchYMD(regex_t *regex, const char *pattern, const char *source, date_t *date) {
#define CAPTURES 3
    regfree(regex);
    regmatch_t matches[CAPTURES + 1];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0) return 0;
    if (regexec(regex, source, CAPTURES + 1, matches, 0) != 0) return 0;

    sprintf(temp, "%.*s", matches[1].rm_eo - matches[1].rm_so, source + matches[1].rm_so);
    temp_date.year = atoi(temp); // NOLINT(*-err34-c)
    sprintf(temp, "%.*s", matches[2].rm_eo - matches[2].rm_so, source + matches[2].rm_so);
    temp_date.month = atoi(temp); // NOLINT(*-err34-c)
    sprintf(temp, "%.*s", matches[3].rm_eo - matches[3].rm_so, source + matches[3].rm_so);
    temp_date.day = atoi(temp); // NOLINT(*-err34-c)

    if (temp_date.year < 100) temp_date.year += 2000;
    if (temp_date.day > 31 || temp_date.month > 12 || temp_date.year > 2099 || temp_date.year < 2000) return 0;
    memcpy(date, &temp_date, sizeof(date_t));
    return 1;
#undef CAPTURES
}

int searchDMY(regex_t *regex, const char *pattern, const char *source, date_t *date) {
#define CAPTURES 3
    regfree(regex);
    regmatch_t matches[CAPTURES + 1];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0) return 0;
    if (regexec(regex, source, CAPTURES + 1, matches, 0) != 0) return 0;

    sprintf(temp, "%.*s", matches[1].rm_eo - matches[1].rm_so, source + matches[1].rm_so);
    temp_date.day = atoi(temp); // NOLINT(*-err34-c)
    sprintf(temp, "%.*s", matches[2].rm_eo - matches[2].rm_so, source + matches[2].rm_so);
    temp_date.month = atoi(temp); // NOLINT(*-err34-c)
    sprintf(temp, "%.*s", matches[3].rm_eo - matches[3].rm_so, source + matches[3].rm_so);
    temp_date.year = atoi(temp); // NOLINT(*-err34-c)

    if (temp_date.year < 100) temp_date.year += 2000;
    if (temp_date.day > 31 || temp_date.month > 12 || temp_date.year > 2099 || temp_date.year < 2000) return 0;
    memcpy(date, &temp_date, sizeof(date_t));
    return 1;
#undef CAPTURES
}

int searchDM(regex_t *regex, const char *pattern, const char *source, date_t *date) {
#define CAPTURES 2
    regfree(regex);
    regmatch_t matches[CAPTURES + 1];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0) return 0;
    if (regexec(regex, source, CAPTURES + 1, matches, 0) != 0) return 0;

    sprintf(temp, "%.*s", matches[1].rm_eo - matches[1].rm_so, source + matches[1].rm_so);
    temp_date.day = atoi(temp); // NOLINT(*-err34-c)
    sprintf(temp, "%.*s", matches[2].rm_eo - matches[2].rm_so, source + matches[2].rm_so);
    temp_date.month = atoi(temp); // NOLINT(*-err34-c)

    if (temp_date.day > 31 || temp_date.month > 12) return 0;
    time_t current_time;
    time(&current_time);
    struct tm *local_time = localtime(&current_time);
    temp_date.year = local_time->tm_year + 1900;
    memcpy(date, &temp_date, sizeof(date_t));
    return 1;
#undef CAPTURES
}

int searchMD(regex_t *regex, const char *pattern, const char *source, date_t *date) {
#define CAPTURES 2
    regfree(regex);
    regmatch_t matches[CAPTURES + 1];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0) return 0;
    if (regexec(regex, source, CAPTURES + 1, matches, 0) != 0) return 0;

    sprintf(temp, "%.*s", matches[1].rm_eo - matches[1].rm_so, source + matches[1].rm_so);
    temp_date.month = atoi(temp); // NOLINT(*-err34-c)
    sprintf(temp, "%.*s", matches[2].rm_eo - matches[2].rm_so, source + matches[2].rm_so);
    temp_date.day = atoi(temp); // NOLINT(*-err34-c)

    if (temp_date.day > 31 || temp_date.month > 12) return 0;
    time_t current_time;
    time(&current_time);
    struct tm *local_time = localtime(&current_time);
    temp_date.year = local_time->tm_year + 1900;
    memcpy(date, &temp_date, sizeof(date_t));
    return 1;
#undef CAPTURES
}

/*
 * FORMATS:
 *
 * [YYYY|YY][\| |-][MM|M][\| |-][DD|D]
 * [YYYY|YY]MMDD
 *
 * [DD|D][\| |-][MM|M][\| |-][YYYY|YY]
 * DDMM[YYYY|YY]
 *
 * [DD|D][\| |-][MM|M]
 * DDMM
 *
 * [MM|M][\| |-][DD|D]
 * MMDD
 *
 * *months with alpha chars
 */
