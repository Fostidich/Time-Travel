//
// Created by kello on 04/04/24.
//

/**
 * NOTES:
 * - american formatted dates are unrecognised;
 * - years are valid if between 2000 and 2099;
 * - if a filename has 2 dates, only the first one is considered.
 */

#ifndef TIME_TRAVEL_MAIN_H
#define TIME_TRAVEL_MAIN_H

#define CP printf("CP "); fflush(stdout);
#define PS(str) printf("%s ", str); fflush(stdout);
#define PI(var) printf("%d ", var); fflush(stdout);

#ifdef TEST
int main1(int argc, char **argv);
#else
int main(int argc, char **argv);
#endif

#endif //TIME_TRAVEL_MAIN_H
