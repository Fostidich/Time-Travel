//
// Created by kello on 04/04/24.
//

#include <string.h>
#include <ctype.h>
#include "finder.h"
#include "main.h"

int findDate(char* dest, const char *filename) {
    if (strlen(filename) > FILENAME_LEN - 1) {
        return -1; // Destination buffer overflow
    }

    int num_letters = 0;
    int all_uppercase = 1; // Assume all uppercase initially

    for (int i = 0; filename[i] != '\0'; i++) {
        if (isalpha(filename[i])) {
            num_letters++;
            if (islower(filename[i])) {
                all_uppercase = 0;
                dest[i] = toupper(filename[i]);
            } else {
                dest[i] = filename[i]; // Already uppercase
            }
        } else {
            dest[i] = filename[i]; // Copy non-letter characters
        }
    }

    dest[FILENAME_LEN - 1] = '\0'; // Ensure null termination in case of overflow

    if (num_letters == 0) {
        return -2; // No letters present
    } else if (all_uppercase) {
        return -1; // All letters already uppercase
    } else {
        return 0;  // Conversion successful
    }
}

// TODO: check if name is already ok