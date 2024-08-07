#define PCRE2_CODE_UNIT_WIDTH 8
#include <stdio.h>
#include <pcre2.h>
#include <string.h>

#include "files.h"
#include "finder.h"

#define MONTH_LANGS 4

const char *months[12][MONTH_LANGS] = {
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
    pcre2_code *re;  // compiled regular expression
    PCRE2_SPTR pattern = (PCRE2_SPTR) "^\\d{4}-\\d{2}-\\d{2}$";
    PCRE2_SPTR subject = (PCRE2_SPTR)filename;
    int err_number;
    PCRE2_SIZE err_offset;
    pcre2_match_data *match_data;
    int rc;  // match return code
    struct date date;

    // Compile and check compilation outcome
    re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &err_number, &err_offset, NULL);
    if (re == NULL) return FAILURE;

    // Perform the match
    match_data = pcre2_match_data_create_from_pattern(re, NULL);
    rc = pcre2_match(re, subject, strlen((char *)subject), 0, 0, match_data, NULL);

    // Manage match errors / absence
    if (rc < 4) {
        pcre2_match_data_free(match_data);
        pcre2_code_free(re);
        return (rc >= PCRE2_ERROR_NOMATCH) ? UNKNOWN : FAILURE;
    }

    // Find matches position
    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
    char *endptr;

    // Find year
    int year_len = ovector[3] - ovector[2];
    char year_str[year_len + 1];
    strncpy(year_str, (char *)subject + ovector[2], year_len);
    year_str[year_len] = '\0';
    date.year = strtol(year_str, &endptr, 10);
    if (endptr == year_str) return UNKNOWN;

    // Find month
    int month_len = ovector[5] - ovector[4];
    char month_str[month_len + 1];
    strncpy(month_str, (char *)subject + ovector[4], month_len);
    month_str[month_len] = '\0';
    date.month = strtol(month_str, &endptr, 10);
    if (endptr == month_str) return UNKNOWN;

    // Find day
    int day_len = ovector[7] - ovector[6];
    char day_str[day_len + 1];
    strncpy(day_str, (char *)subject + ovector[6], day_len);
    day_str[day_len] = '\0';
    date.day = strtol(day_str, &endptr, 10);
    if (endptr == day_str) return UNKNOWN;

    if (!check_date(date)) return UNKNOWN;

    // Produce final string
    snprintf(dest, NAME_MAX, "%04d-%02d-%02d", date.year, date.month, date.day);

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
    return FOUND;
}

bool check_date(struct date date) {
    if (date.year == 0 || date.month == 0 || date.day == 0) return false;
    if (date.year > 4000 || date.month > 12 || date.day > 31) return false;

    const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int isLeap = date.year % 4 == 0 && (date.year % 100 != 0 || date.year % 400 == 0);

    if (isLeap && date.month == 2 && date.day > 29) return false;
    if (date.day > daysInMonth[date.month]) return false;

    return true;
}
