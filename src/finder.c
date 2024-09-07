#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "files.h"
#include "finder.h"

#define MONTH_LANGS 4
#define MAX_FUTURE_YEAR 10
#define MAX_PATTERN 256

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

const char *format = "%04d-%02d-%02d";

int find_date(char *dest, const char *source) {
#define switch_find(v)                               \
    {                                                \
        switch (v) {                                 \
            case FAILURE:                            \
                return FAILURE;                      \
            case FOUND:                              \
                if (found++)                         \
                    return UNSURE;                   \
                else                                 \
                    strncpy(dest, result, NAME_MAX); \
        }                                            \
    }

#define cycle_months(f, s, c, y, d)                                                          \
    {                                                                                        \
        for (int m = 1; m <= 12; m++) {                                                      \
            strncpy(pattern, f, MAX_PATTERN);                                                \
            strncat(pattern, months[m - 1][l], MAX_PATTERN - strnlen(pattern, MAX_PATTERN)); \
            strncat(pattern, s, MAX_PATTERN - strnlen(pattern, MAX_PATTERN));                \
            switch_find(execute_and_extract(result, source, pattern, c, y, -1, d, m));       \
        }                                                                                    \
    }

    int found = 0;
    char result[NAME_MAX];

    // Look for dates with worded month, including year; symbol between values are not mandatory
    for (int l = 0; l < MONTH_LANGS; l++) {
        char pattern[MAX_PATTERN];

        // YYYY-month-DD // YYYY-month-D // YY-month-DD // YY-month-D
        cycle_months(R"((?:\D|^)(\d{4}|\d{2})[- _]?(?i)(?:)", R"()[- _]?(\d{2}|\d{1})(?:\D|$))", 2, 0, 1);
        // DD-month-YYYY // DD-month-YY // D-month-YYYY // D-month-YY
        cycle_months(R"((?:\D|^)(\d{2}|\d{1})[- _]?(?i)(?:)", R"()[- _]?(\d{4}|\d{2})(?:\D|$))", 2, 1, 0);

        if (found) goto END;
    }

    // Look for dates with worded month, excluding year; symbol between values are not mandatory
    for (int l = 0; l < MONTH_LANGS; l++) {
        char pattern[MAX_PATTERN];

        // month-DD // month-D
        cycle_months(R"((?i)(?:)", R"()[- _]?(\d{2}|\d{1})(?:\D|$))", 1, -1, 0);
        // DD-month // D-month
        cycle_months(R"((?:\D|^)(\d{2}|\d{1})[- _]?(?i)(?:)", R"())", 1, -1, 0);

        if (found) goto END;
    }

    // Look for dates with numerical month, including year; symbol between values are either mandatory or not
    {
        // YYYY-MM-DD // YYYY-MM-D // YYYY-M-DD // YYYY-M-D // YY-MM-DD // YY-MM-D // YY-M-DD // YY-M-D
        switch_find(execute_and_extract(result, source, R"((?:\D|^)(\d{4}|\d{2})[- _](\d{2}|\d{1})[- _](\d{2}|\d{1})(?:\D|$))", 3, 0, 1, 2, -1));
        // DD-MM-YYYY // DD-MM-YY // DD-M-YYYY // DD-M-YY // D-MM-YYYY // D-MM-YY // D-M-YYYY // D-M-YY
        switch_find(execute_and_extract(result, source, R"((?:\D|^)(\d{2}|\d{1})[- _](\d{2}|\d{1})[- _](\d{4}|\d{2})(?:\D|$))", 3, 2, 1, 0, -1));

        // YYYYMMDD // YYMMDD
        switch_find(execute_and_extract(result, source, R"((?:\D|^)(\d{4}|\d{2})(\d{2})(\d{2})(?:\D|$))", 3, 0, 1, 2, -1));
        // DDMMYYYY // DDMMYY
        switch_find(execute_and_extract(result, source, R"((?:\D|^)(\d{2})(\d{2})(\d{4}|\d{2})(?:\D|$))", 3, 2, 1, 0, -1));
    }

    if (found) goto END;

    // Look for dates with numerical month, excluding year; symbol between values are either mandatory or not
    {
        // MM-DD // MM-D // M-DD // M-D
        switch_find(execute_and_extract(result, source, R"((?:\D|^)(\d{2}|\d{1})[- _](\d{2}|\d{1})(?:\D|$))", 2, -1, 0, 1, -1));
        // DD-MM // DD-M // D-MM // D-M
        switch_find(execute_and_extract(result, source, R"((?:\D|^)(\d{2}|\d{1})[- _](\d{2}|\d{1})(?:\D|$))", 2, -1, 1, 0, -1));

        // MMDD
        switch_find(execute_and_extract(result, source, R"((?:\D|^)(\d{2})(\d{2})(?:\D|$))", 2, -1, 0, 1, -1));
        // DDMM
        switch_find(execute_and_extract(result, source, R"((?:\D|^)(\d{2})(\d{2})(?:\D|$))", 2, -1, 1, 0, -1));
    }

END:  // return switch
    if (!found) return UNKNOWN;
    if (strncmp(dest, source, NAME_MAX) == 0) return UNCHANGED;
    return FOUND;

#undef cycle_months
#undef switch_find
}

int execute_and_extract(char *dest, const char *source, const char *pattern, int captures, int yp, int mp, int dp, int mv) {
    // Find matches position
    int err_code;
    int *ovector = execute_regex(pattern, source, captures, &err_code);
    if (err_code == 0) return UNKNOWN;
    if (err_code == -1) return FAILURE;

    // Find date
    struct date date;
    date.year = (yp == -1) ? this_year() : extract_date_value(ovector, source, yp);
    date.month = (mp == -1) ? mv : extract_date_value(ovector, source, mp);
    date.day = extract_date_value(ovector, source, dp);
    free(ovector);

    // Allow year-unexplicit future dates only if in range
    if (date.year < 100) date.year += (date.year <= this_year() - 2000 + MAX_FUTURE_YEAR) ? 2000 : 1900;

    // Date validity check
    if (!check_date(date)) return UNKNOWN;

    // Produce final string
    snprintf(dest, NAME_MAX, format, date.year, date.month, date.day);
    return FOUND;
}

int extract_date_value(const int *ovector, const char *source, int pos) {
    pos++;
    char *endptr;
    int len = ovector[2 * pos + 1] - ovector[2 * pos];
    char str[len + 1];
    strncpy(str, source + ovector[2 * pos], len);
    str[len] = '\0';
    int res = strtol(str, &endptr, 10);
    return (endptr == str) ? -1 : res;
}

int *execute_regex(const char *pattern, const char *subject, int captures, int *error) {
    int err_number;
    PCRE2_SIZE err_offset;

    pcre2_code *re;                // compiled regular expression
    pcre2_match_data *match_data;  // captures information
    int rc;                        // match return code

    // Compile and check compilation outcome
    re = pcre2_compile((PCRE2_SPTR)pattern, PCRE2_ZERO_TERMINATED, 0, &err_number, &err_offset, NULL);
    if (re == NULL) {
        *error = -1;
        return NULL;
    }

    // Perform the match
    match_data = pcre2_match_data_create_from_pattern(re, NULL);
    rc = pcre2_match(re, (PCRE2_SPTR)subject, strlen(subject), 0, 0, match_data, NULL);

    // Manage match errors / absence
    if (rc < captures + 1) {
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

int this_year() {
    time_t t;
    struct tm *tm_info;
    int year;
    time(&t);
    tm_info = localtime(&t);
    return tm_info->tm_year + 1900;
}

bool check_date(struct date date) {
    if (date.year == 0 || date.month == 0 || date.day == 0) return false;
    if (date.year > this_year() + MAX_FUTURE_YEAR || date.month > 12 || date.day > 31) return false;
    const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (date.month == 2) {
        int isLeap = date.year % 4 == 0 && (date.year % 100 != 0 || date.year % 400 == 0);
        if (isLeap && date.day <= 29) return true;
    }
    if (date.day > daysInMonth[date.month]) return false;
    return true;
}
