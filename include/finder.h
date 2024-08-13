#ifndef FINDER_H
#define FINDER_H

struct date {
    unsigned short day;
    unsigned short month;
    unsigned short year;
};

int find_date(char *dest, const char *source);

bool check_date(struct date date);

#endif
