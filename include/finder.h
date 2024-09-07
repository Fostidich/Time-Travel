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
// The returned array is to be freed.
int *execute_regex(const char *pattern, const char *subject, int catches, int *error);

// Return the int value of the number in the source string at the provided capturing position, using
// the ovector to find its limits
int extract_date_value(const int *ovector, const char *source, int pos);

// Find the date contained in the source string by providing the regex pattern and the number of
// captures. The position order of numerical values (starting from 0) of the date are to be pointed
// with yp, mp and dp. If a parameter (e.g. the year or the month) is not considered, use -1 as argument.
// If the date is matched with worded month, the month value is to be specified with mv.
int execute_and_extract(char *dest, const char *source, const char *pattern, int captures, int yp, int mp, int dp, int mv);

#endif
