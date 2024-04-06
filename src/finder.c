//
// Created by kello on 04/04/24.
//

#include <string.h>
#include "finder.h"
#include "main.h"

int findDate(char *dest, const char *filename) {
    // if same length as date return unchanged
    if (strlen(filename) == DATE_LEN) {
        strcpy(dest, filename);
        return DATE_FOUND;
    }

    // if starts with uppercase return unknown
    if (filename[0] >= 'A' && filename[0] <= 'Z')
        return DATE_UNKNOWN;

    // if starts with lowercase return unsure
    if (filename[0] >= 'a' && filename[0] <= 'z') {
        strncpy(dest, filename, 3);
        strcpy(dest + 3, "-unsure");
        return DATE_UNSURE;
    }

    // if starts with digit (otherwise) return found
    strncpy(dest, filename, 3);
    strcpy(dest + 3, "--found");
    return DATE_FOUND;
}

// TODO: check if name is already ok
