#ifndef FINDER_H
#define FINDER_H

#define FOUND 0
#define UNSURE 1
#define UNKNOWN 2
#define UNCHANGED 3
#define FAILURE 4

struct date {
    unsigned short day;
    unsigned short month;
    unsigned short year;
};

int find_date(char *dest, const char *source);
bool check_date(struct date date);

#endif
