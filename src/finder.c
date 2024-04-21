//
// Created by kello on 04/04/24.
//

#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "finder.h"
#include "main.h"

// TODO: remember to find months in word format
// TODO: add a year in the struct if it's not to be found
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
    found += searchYMD(regex, "([[:digit:]]{2})([[:digit:]]{2})([[:digit:]]{2})", filename, date);
    found += searchYMD(regex, "20([[:digit:]]{2})([[:digit:]]{2})([[:digit:]]{2})", filename, date);
    found += searchYMD(regex, "([[:digit:]]{2}) ([[:digit:]]{1,2}) ([[:digit:]]{1,2})", filename, date);
    found += searchYMD(regex, "([[:digit:]]{2})\\([[:digit:]]{1,2})\\([[:digit:]]{1,2})", filename, date);
    found += searchYMD(regex, "([[:digit:]]{2})-([[:digit:]]{1,2})-([[:digit:]]{1,2})", filename, date);

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
    regfree(regex);
    regmatch_t matches[4];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0) return 0;
    if (regexec(regex, source, 4, matches, 0) != 0) return 0;

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
}

void searchDMY(char *pattern) {

}

void searchDM(char *pattern) {

}

void searchMD(char *pattern) {

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
