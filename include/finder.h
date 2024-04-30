//
// Created by kello on 04/04/24.
//

#ifndef TIME_TRAVEL_FINDER_H
#define TIME_TRAVEL_FINDER_H

#include <regex.h>

#define DATE_LEN 10
#define DATE_FOUND 0
#define DATE_UNSURE (-1)
#define DATE_UNKNOWN (-2)

#define MONTH_LANG 4

typedef struct date {
    unsigned short day;
    unsigned short month;
    unsigned short year;
} date_t;

extern const char *months[12][MONTH_LANG];

int find_date(char *dest, const char *filename);

int searchDmY(regex_t *regex, const char *pattern, const char *source, date_t *date, int month);
int searchYmD(regex_t *regex, const char *pattern, const char *source, date_t *date, int month);
int searchDm(regex_t *regex, const char *pattern, const char *source, date_t *date, int month);
int searchmD(regex_t *regex, const char *pattern, const char *source, date_t *date, int month);
int searchDMY(regex_t *regex, const char *pattern, const char *source, date_t *date);
int searchYMD(regex_t *regex, const char *pattern, const char *source, date_t *date);
int searchMD(regex_t *regex, const char *pattern, const char *source, date_t *date);
int searchDM(regex_t *regex, const char *pattern, const char *source, date_t *date);

int check_date(const date_t *date);

#endif //TIME_TRAVEL_FINDER_H
