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

#define MAX_LEN 256

// TODO Surely these macros are tautological

#define DmY(pat) found_tmp = 0; \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "^" pat "$" , months[j][i]); \
                        if (searchDmY(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "^" pat "[^[:digit:]]", months[j][i]); \
                        if (searchDmY(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "[^[:digit:]]" pat "$", months[j][i]); \
                        if (searchDmY(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "[^[:digit:]]" pat "[^[:digit:]]", months[j][i]); \
                        if (searchDmY(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \

#define YmD(pat) found_tmp = 0; \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "^" pat "$" , months[j][i]); \
                        if (searchYmD(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "^" pat "[^[:digit:]]", months[j][i]); \
                        if (searchYmD(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "[^[:digit:]]" pat "$", months[j][i]); \
                        if (searchYmD(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "[^[:digit:]]" pat "[^[:digit:]]", months[j][i]); \
                        if (searchYmD(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \

#define Dm(pat) found_tmp = 0; \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "^" pat, months[j][i]); \
                        if (searchDm(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, "[^[:digit:]]" pat, months[j][i]); \
                        if (searchDm(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \

#define mD(pat) found_tmp = 0; \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, pat "$" , months[j][i]); \
                        if (searchmD(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \
                for (int i = 0; i < MONTH_LANG && found_tmp == 0; i++) { \
                    for (int j = 0; j < 12; j++) { \
                        sprintf(temp, pat "[^[:digit:]]", months[j][i]); \
                        if (searchmD(regex, temp, lc_filename, date, j + 1)) {found_tmp++; found++; break;};}} \

#define DMY(str) found += searchDMY(regex, "^" str "$", lc_filename, date); \
                found += searchDMY(regex, "^" str "[^[:digit:]]", lc_filename, date); \
                found += searchDMY(regex, "[^[:digit:]]" str "$", lc_filename, date); \
                found += searchDMY(regex, "[^[:digit:]]" str "[^[:digit:]]", lc_filename, date);

#define YMD(str) found += searchYMD(regex, "^" str "$", lc_filename, date); \
                found += searchYMD(regex, "^" str "[^[:digit:]]", lc_filename, date); \
                found += searchYMD(regex, "[^[:digit:]]" str "$", lc_filename, date); \
                found += searchYMD(regex, "[^[:digit:]]" str "[^[:digit:]]", lc_filename, date);

#define DM(str) found += searchDM(regex, "^" str "$", lc_filename, date); \
                found += searchDM(regex, "^" str "[^[:digit:]]", lc_filename, date); \
                found += searchDM(regex, "[^[:digit:]]" str "$", lc_filename, date); \
                found += searchDM(regex, "[^[:digit:]]" str "[^[:digit:]]", lc_filename, date);

#define MD(str) found += searchMD(regex, "^" str "$", lc_filename, date); \
                found += searchMD(regex, "^" str "[^[:digit:]]", lc_filename, date); \
                found += searchMD(regex, "[^[:digit:]]" str "$", lc_filename, date); \
                found += searchMD(regex, "[^[:digit:]]" str "[^[:digit:]]", lc_filename, date);

const char *months[12][MONTH_LANG] = {
        {"january",   "jan", "gennaio",   "gen"},
        {"february",  "feb", "febbraio",  "feb"},
        {"march",     "mar", "marzo",     "mar"},
        {"april",     "apr", "aprile",    "apr"},
        {"may",       "may", "maggio",    "mag"},
        {"june",      "jun", "giugno",    "giu"},
        {"july",      "jul", "luglio",    "lug"},
        {"august",    "aug", "agosto",    "ago"},
        {"september", "sep", "settembre", "set"},
        {"october",   "oct", "ottobre",   "ott"},
        {"november",  "nov", "novembre",  "nov"},
        {"december",  "dec", "dicembre",  "dic"}
};

int find_date(char *dest, const char *filename) {
    int found = 0, found_tmp = 0;
    char temp[MAX_LEN];
    char lc_filename[strlen(filename) + 1];

    for (int i = 0; filename[i] != '\0'; i++)
        lc_filename[i] = filename[i] >= 'A' && filename[i] <= 'Z' ? filename[i] + 32 : filename[i];
    lc_filename[strlen(filename)] = '\0';

    regex_t *regex = malloc(sizeof(regex_t));
    regcomp(regex, "", 0);
    date_t *date = calloc(1, sizeof(date_t));

    /*
     * Searches in each section are from less likely to most likely.
     * Sections:
     * 1. dates with month in literal format (along with both year and day);
     * 2. dates with month in literal format (along with just the day);
     * 3. dates made of three numbers (year, month, day);
     * 4. dates made of two numbers (no year).
     */

    // TODO These regexes could be definitely compacted and optimized using PCREv2 but as for now idc... \
            ...or does someone know how to use option groups, i.e. make "(?:a|b|c)" or "[a|b|c]" valid?
    {
        DmY("([[:digit:]]{1,2})%s([[:digit:]]{2})") // 21apr24
        DmY("([[:digit:]]{1,2})%s20([[:digit:]]{2})") // 21apr2024
        DmY("([[:digit:]]{1,2})[[:punct:]]%s[[:punct:]]([[:digit:]]{2})") // 21-apr-24
        DmY("([[:digit:]]{1,2}) %s ([[:digit:]]{2})") // 21 apr 24
        DmY("([[:digit:]]{1,2})[[:punct:]]%s[[:punct:]]20([[:digit:]]{2})") // 21-apr-2024
        DmY("([[:digit:]]{1,2}) %s 20([[:digit:]]{2})") // 21 apr 2024

        YmD("([[:digit:]]{2})%s([[:digit:]]{1,2})") // 24apr21
        YmD("20([[:digit:]]{2})%s([[:digit:]]{1,2})") // 2024apr21
        YmD("([[:digit:]]{2})[[:punct:]]%s[[:punct:]]([[:digit:]]{1,2})") // 24-apr-21
        YmD("([[:digit:]]{2}) %s ([[:digit:]]{1,2})") // 24 apr 21
        YmD("20([[:digit:]]{2})[[:punct:]]%s[[:punct:]]([[:digit:]]{1,2})") // 2024-apr-21
        YmD("20([[:digit:]]{2}) %s ([[:digit:]]{1,2})") // 2024 apr 21
    }
    if (found > 0) goto FINISH;
    {
        Dm("([[:digit:]]{1,2})%s") // 22apr
        Dm("([[:digit:]]{1,2})[[:punct:]]%s") // 22-apr
        Dm("([[:digit:]]{1,2}) %s") // 22 apr

        mD("%s([[:digit:]]{1,2})") // apr22
        mD("%s[[:punct:]]([[:digit:]]{1,2})") // apr-22
        mD("%s ([[:digit:]]{1,2})") // apr 22
    }
    if (found > 0) goto FINISH;
    {
        DMY("([[:digit:]]{2})([[:digit:]]{2})([[:digit:]]{2})") // 210424
        DMY("([[:digit:]]{2})([[:digit:]]{2})20([[:digit:]]{2})") // 21042024
        DMY("([[:digit:]]{1,2})[[:punct:]]([[:digit:]]{1,2})[[:punct:]]([[:digit:]]{2})") // 21-04-24
        DMY("([[:digit:]]{1,2}) ([[:digit:]]{1,2}) ([[:digit:]]{2})") // 21 04 24
        DMY("([[:digit:]]{1,2})[[:punct:]]([[:digit:]]{1,2})[[:punct:]]20([[:digit:]]{2})") // 21-04-2024
        DMY("([[:digit:]]{1,2}) ([[:digit:]]{1,2}) 20([[:digit:]]{2})") // 21 04 2024

        YMD("([[:digit:]]{2})([[:digit:]]{2})([[:digit:]]{2})") // 240421
        YMD("20([[:digit:]]{2})([[:digit:]]{2})([[:digit:]]{2})") // 20240421
        YMD("([[:digit:]]{2})[[:punct:]]([[:digit:]]{1,2})[[:punct:]]([[:digit:]]{1,2})") // 24-04-21
        YMD("([[:digit:]]{2}) ([[:digit:]]{1,2}) ([[:digit:]]{1,2})") // 24 04 21
        YMD("20([[:digit:]]{2})[[:punct:]]([[:digit:]]{1,2})[[:punct:]]([[:digit:]]{1,2})") // 2024-04-21
        YMD("20([[:digit:]]{2}) ([[:digit:]]{1,2}) ([[:digit:]]{1,2})") // 2024 04 21
    }
    if (found > 0) goto FINISH;
    {
        DM("([[:digit:]]{2})([[:digit:]]{2})") // 2204
        DM("([[:digit:]]{1,2})[[:punct:]]([[:digit:]]{1,2})") // 22-04
        DM("([[:digit:]]{1,2}) ([[:digit:]]{1,2})") // 22 04

        MD("([[:digit:]]{2})([[:digit:]]{2})") // 0422
        MD("([[:digit:]]{1,2})[[:punct:]]([[:digit:]]{1,2})") // 04-22
        MD("([[:digit:]]{1,2}) ([[:digit:]]{1,2})") // 04 22
    }
    FINISH:

    sprintf(dest, "%04d-%02d-%02d", date->year, date->month, date->day);
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

// TODO One day I might compress the following functions in a single one

int searchDmY(regex_t *regex, const char *pattern, const char *source, date_t *date, int month) {
#define CAPTURES 2
    regfree(regex);
    regmatch_t matches[CAPTURES + 1];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0)  return 0;
    if (regexec(regex, source, CAPTURES + 1, matches, 0) != 0) return 0;

    sprintf(temp, "%.*s", matches[1].rm_eo - matches[1].rm_so, source + matches[1].rm_so);
    temp_date.day = atoi(temp); // NOLINT(*-err34-c)
    sprintf(temp, "%.*s", matches[2].rm_eo - matches[2].rm_so, source + matches[2].rm_so);
    temp_date.year = atoi(temp); // NOLINT(*-err34-c)

    if (temp_date.year < 100) temp_date.year += 2000;
    if (temp_date.day > 31 || temp_date.year > 2099 || temp_date.year < 2000) return 0;
    temp_date.month = month;
    memcpy(date, &temp_date, sizeof(date_t));
    return 1;
#undef CAPTURES
}

int searchYmD(regex_t *regex, const char *pattern, const char *source, date_t *date, int month) {
#define CAPTURES 2
    regfree(regex);
    regmatch_t matches[CAPTURES + 1];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0) return 0;
    if (regexec(regex, source, CAPTURES + 1, matches, 0) != 0) return 0;

    sprintf(temp, "%.*s", matches[1].rm_eo - matches[1].rm_so, source + matches[1].rm_so);
    temp_date.year = atoi(temp); // NOLINT(*-err34-c)
    sprintf(temp, "%.*s", matches[2].rm_eo - matches[2].rm_so, source + matches[2].rm_so);
    temp_date.day = atoi(temp); // NOLINT(*-err34-c)

    if (temp_date.year < 100) temp_date.year += 2000;
    if (temp_date.day > 31 || temp_date.year > 2099 || temp_date.year < 2000) return 0;
    temp_date.month = month;
    memcpy(date, &temp_date, sizeof(date_t));
    return 1;
#undef CAPTURES
}

int searchDm(regex_t *regex, const char *pattern, const char *source, date_t *date, int month) {
#define CAPTURES 1
    regfree(regex);
    regmatch_t matches[CAPTURES + 1];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0) return 0;
    if (regexec(regex, source, CAPTURES + 1, matches, 0) != 0) return 0;

    sprintf(temp, "%.*s", matches[1].rm_eo - matches[1].rm_so, source + matches[1].rm_so);
    temp_date.day = atoi(temp); // NOLINT(*-err34-c)

    if (temp_date.day > 31) return 0;
    time_t current_time;
    time(&current_time);
    struct tm *local_time = localtime(&current_time);
    temp_date.year = local_time->tm_year + 1900;
    temp_date.month = month;
    memcpy(date, &temp_date, sizeof(date_t));
    return 1;
#undef CAPTURES
}

int searchmD(regex_t *regex, const char *pattern, const char *source, date_t *date, int month) {
#define CAPTURES 1
    regfree(regex);
    regmatch_t matches[CAPTURES + 1];
    char temp[12];
    date_t temp_date;
    if (regcomp(regex, pattern, REG_EXTENDED) != 0) return 0;
    if (regexec(regex, source, CAPTURES + 1, matches, 0) != 0) return 0;

    sprintf(temp, "%.*s", matches[1].rm_eo - matches[1].rm_so, source + matches[1].rm_so);
    temp_date.day = atoi(temp); // NOLINT(*-err34-c)

    if (temp_date.day > 31) return 0;
    time_t current_time;
    time(&current_time);
    struct tm *local_time = localtime(&current_time);
    temp_date.year = local_time->tm_year + 1900;
    temp_date.month = month;
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
