//
// Created by kello on 04/04/24.
//

#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "finder.h"
#include "main.h"

int findDate(char *dest, const char *filename) {
    if (strlen(filename) > FILENAME_LEN - 1) {
        return -1; // Destination buffer overflow
    }

    bool has_letters = false;
    bool has_digits = false;

    int i = 0;
    while (filename[i] != '\0') {
        if (isalpha(filename[i])) {
            has_letters = true;
            if (islower(filename[i])) {
                dest[i] = toupper(filename[i]); // NOLINT(*-narrowing-conversions)
            } else {
                dest[i] = filename[i];
            }
        } else if (isdigit(filename[i])) {
            has_digits = true;
            dest[i] = filename[i];
        } else {
            dest[i] = filename[i];
        }
        i++;
    }

    dest[i] = '\0';

    if (!has_letters) {
        return DATE_UNKNOWN; // No letters present
    } else if (!has_digits) {
        return DATE_FOUND; // Only letters present
    } else {
        return DATE_UNSURE;  // Conversion successful (letters and digits or just digits)
    }
}


// TODO: check if name is already ok