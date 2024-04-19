//
// Created by kello on 04/04/24.
//

#ifndef TIME_TRAVEL_FINDER_H
#define TIME_TRAVEL_FINDER_H

#define DATE_LEN 10
#define DATE_FOUND 0
#define DATE_UNSURE (-1)
#define DATE_UNKNOWN (-2)

extern const char *months[4][12] = {
        {"january", "february", "march", "april",  "may",    "june",   "july",   "august", "september", "october", "november", "december"},
        {"jan",     "feb",      "mar",   "apr",    "may",    "jun",    "jul",    "aug",    "sep",       "oct",     "nov",      "dec"},
        {"gennaio", "febbraio", "marzo", "aprile", "maggio", "giugno", "luglio", "agosto", "settembre", "ottobre", "novembre", "dicembre"},
        {"gen",     "feb",      "mar",   "apr",    "mag",    "giu",    "lug",    "ago",    "set",       "ott",     "nov",      "dic"}
};

int findDate(char *dest, const char *filename);

#endif //TIME_TRAVEL_FINDER_H
