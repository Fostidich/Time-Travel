#ifndef FINDER_H
#define FINDER_H

struct date {
    unsigned short day;
    unsigned short month;
    unsigned short year;
};

int find_date(char *dest, const char *source);

bool check_date(struct date date);
int this_year();

// Regex is compiled and executed. The returned array contains the subject indexes corresponding to
// the identified matches (ovector).
// Returns 1 for success, 0 for no match, -1 for failure.
// The returned array is to bee freed.
int *execute_regex(const char *pattern, const char *subject, int catches, int *error);

int extract_date_value(const int *ovector, const char *source, int pos);

int find_numerical_date(
    char *dest, const char *source, const char *pattern, int captures, int yp, int mp, int dp);

#endif
