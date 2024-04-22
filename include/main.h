//
// Created by kello on 04/04/24.
//

#ifndef TIME_TRAVEL_MAIN_H
#define TIME_TRAVEL_MAIN_H

#define CP printf("CP "); fflush(stdout);
#define PV(str) printf("%s ", str); fflush(stdout);

#ifdef TEST
int main1(int argc, char **argv);
#else
int main(int argc, char **argv);
#endif

#endif //TIME_TRAVEL_MAIN_H
