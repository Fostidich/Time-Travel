#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <stdio.h>
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

int find_date(char *dest, const char *source) {
    // Find matches position
    int err_code;
    int *ovector = execute_regex("(\\d{4})-(\\d{2})-(\\d{2})", source, 3, &err_code);
    if (err_code == 0) return UNKNOWN;
    if (err_code == -1) return FAILURE;

    // Find date
    struct date date;
    date.year = extract_date_value(ovector, source, 1);
    date.month = extract_date_value(ovector, source, 2);
    date.day = extract_date_value(ovector, source, 3);
    if (!check_date(date)) return UNKNOWN;

    // Produce final string
    snprintf(dest, NAME_MAX, "%04d-%02d-%02d", date.year, date.month, date.day);

    free(ovector);
    return (strncmp(dest, source, NAME_MAX) == 0) ? UNCHANGED : FOUND;
}

int extract_date_value(const int *ovector, const char *source, int pos) {
    char *endptr;
    int len = ovector[2 * pos + 1] - ovector[2 * pos];
    char str[len + 1];
    strncpy(str, source + ovector[2 * pos], len);
    str[len] = '\0';
    int res = strtol(str, &endptr, 10);
    return (endptr == str) ? -1 : res;
}

int *execute_regex(const char *pattern, const char *subject, int catches, int *error) {
    int err_number;
    PCRE2_SIZE err_offset;

    pcre2_code *re;                // compiled regular expression
    pcre2_match_data *match_data;  // captures information
    int rc;                        // match return code

    // Compile and check compilation outcome
    re = pcre2_compile(
        (PCRE2_SPTR)pattern, PCRE2_ZERO_TERMINATED, 0, &err_number, &err_offset, NULL);
    if (re == NULL) {
        *error = -1;
        return NULL;
    }

    // Perform the match
    match_data = pcre2_match_data_create_from_pattern(re, NULL);
    rc = pcre2_match(re, (PCRE2_SPTR)subject, strlen(subject), 0, 0, match_data, NULL);

    // Manage match errors / absence
    if (rc < catches + 1) {
        pcre2_match_data_free(match_data);
        pcre2_code_free(re);
        *error = (rc >= PCRE2_ERROR_NOMATCH) ? 0 : -1;
        return NULL;
    }

    // Find matches position
    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
    int res_size = 2 * pcre2_get_ovector_count(match_data);
    int *res = malloc(res_size * sizeof(int));
    if (res == NULL) {
        *error = -1;
        return NULL;
    }
    for (int i = 0; i < res_size; i++) res[i] = ovector[i];

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
    *error = 1;
    return (int *)res;
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
