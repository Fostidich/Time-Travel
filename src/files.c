#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "files.h"

#define GREY "\033[30m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define WHITE "\033[0m"

int analyze_filenames(const char *folder, DIR *dir, int finder(char *, const char *)) {
    int changes = 0;

    // Cycle through files in the directory
    const struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignore directories
        if (entry->d_type != DT_REG) continue;
        // Ignore hidden files
        if (entry->d_name[0] == '.') continue;
        // Check filename length
        if (strnlen(entry->d_name, NAME_MAX) == NAME_MAX) continue;

        // Initlialize filename source string
        char filename[NAME_MAX];
        strncpy(filename, entry->d_name, NAME_MAX);

        // Look for extension and manage it
        char extension[NAME_MAX] = "";
        char *first_dot = strchr(filename, '.');
        if (first_dot != NULL && first_dot != filename + strnlen(filename, NAME_MAX) - 1) {
            strncpy(extension, first_dot, NAME_MAX);
            *first_dot = '\0';
        }

        // Find the new name with the provided finder function
        char new_name[NAME_MAX] = "";
        int outcome = finder(new_name, filename);

        // Check if changes have actually been made
        if (outcome != UNKNOWN && strncmp(new_name, filename, NAME_MAX) == 0) outcome = UNCHANGED; // FIXME make UNCHANGED check direcly in finder function

        // Look for duplicates in filenames
        if (outcome != UNKNOWN && outcome != UNCHANGED)
            if (!make_filename_unique(folder, new_name, extension)) outcome = FAILURE;

        // Add back extensions to filename and new name
        if (first_dot != NULL) *first_dot = '.';
        strncat(new_name, extension, NAME_MAX - strnlen(new_name, NAME_MAX));
        if (strnlen(new_name, NAME_MAX) == NAME_MAX) outcome = FAILURE;

        if (outcome != UNKNOWN && outcome != UNCHANGED && outcome != FAILURE) {
            // Find old and new file path
            char old_path[PATH_MAX], new_path[PATH_MAX];
            snprintf(old_path, PATH_MAX, "%s/%s", folder, filename);
            snprintf(new_path, PATH_MAX, "%s/%s", folder, new_name);

            // Check path length overflows
            if (strnlen(old_path, PATH_MAX) == PATH_MAX || strnlen(new_path, PATH_MAX) == PATH_MAX)
                outcome = FAILURE;
            else
                // Rename file
                if (rename(old_path, new_path) != 0)
                    outcome = FAILURE;
                else
                    changes++;
        }

        // Show changes
        print_outcome(outcome, filename, new_name);
    }

    return changes;
}

int make_filename_unique(const char *dir, char *filename, const char *extension) {
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s/%s%s", dir, filename, extension);
    if (strnlen(path, PATH_MAX) == PATH_MAX) return 0;

    // Start by checking raw filename
    if (access(path, F_OK) != 0) return errno == ENOENT;

    // Add duplicatation number to filename
    int dupe = 0;
    do {
        dupe++;
        snprintf(path, PATH_MAX, "%s/%s(%d)%s", dir, filename, dupe, extension);
        if (strnlen(path, PATH_MAX) == PATH_MAX) return 0;
    } while (access(path, F_OK) == 0);

    // Check memory access
    if (errno != ENOENT) return 0;

    // Update filename
    char id[16];
    snprintf(id, 16, "(%d)", dupe);
    strncat(filename, id, NAME_MAX - strnlen(filename, NAME_MAX));
    if (strnlen(filename, NAME_MAX) == NAME_MAX) return 0;
    return 1;
}

void print_outcome(int outcome, char *filename, char *new_name) {
    switch (outcome) {
        case FOUND:
            printf("%s -> %s\n", filename, new_name);
            break;
        case UNSURE:
            printf("%s%s -> %s%s\n", YELLOW, filename, new_name, WHITE);
            break;
        case UNKNOWN:
            printf("%sUnknown: %s%s\n", RED, filename, WHITE);
            break;
        case UNCHANGED:
            printf("%s%s -> %s%s\n", GREY, filename, new_name, WHITE);
            break;
        case FAILURE:
            printf("%sFailure: %s%s\n", RED, filename, WHITE);
            break;
    }
}
