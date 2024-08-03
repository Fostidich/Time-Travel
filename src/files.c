#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "files.h"

#define GREY "\033[0;30m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define WHITE "\033[0;37m"

int analyze_filenames(
    const char *folder, DIR *dir, int finder(char *, const char *)) {
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
        strcpy(filename, entry->d_name);

        // Look for extension and manage it
        char extension[NAME_MAX] = "";
        char *first_dot = strchr(filename, '.');
        if (first_dot != NULL && first_dot != filename + strlen(filename) - 1) {
            strcpy(extension, first_dot);
            *first_dot = '\0';
        }

        // Find the new name with the provided finder function
        char new_name[NAME_MAX] = "";
        int outcome = finder(new_name, filename);

        // Check if changes have actually been made
        if (outcome != UNKNOWN && strcmp(new_name, filename) == 0)
            outcome = UNCHANGED;

        // Look for duplicates in filenames
        if (outcome != UNKNOWN && outcome != UNCHANGED)
            if (!make_filename_unique(folder, new_name, extension))
                outcome = UNABLE;

        // Add back extensions to filename and new name
        if (first_dot != NULL) *first_dot = '.';
        strcat(new_name, extension);

        if (outcome != UNKNOWN && outcome != UNCHANGED && outcome != UNABLE) {
            // Find old and new file path
            char old_path[PATH_MAX], new_path[PATH_MAX];
            snprintf(old_path, PATH_MAX, "%s/%s", folder, filename);
            snprintf(new_path, PATH_MAX, "%s/%s", folder, new_name);

            // Rename file
            if (rename(old_path, new_path) != 0) outcome = UNABLE;
        }

        // Show changes
        print_outcome(outcome, filename, new_name);
    }

    return changes;
}

int make_filename_unique(
    const char *dir, char *filename, const char *extension) {
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s/%s%s", dir, filename, extension);

    // Start by checking raw filename
    if (access(path, F_OK) != 0) return errno == ENOENT;

    // Add duplicatation number to filename
    int dupe = 0;
    do {
        dupe++;
        snprintf(path, PATH_MAX, "%s/%s(%d)%s", dir, filename, dupe, extension);
    } while (access(path, F_OK) == 0);

    // Check memory access
    if (errno != ENOENT) return 0;

    // Update filename
    char id[16];
    sprintf(id, "(%d)", dupe);
    strncat(filename, id, NAME_MAX);
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
            printf("%sUNKNOWN DATE: %s%s\n", RED, filename, WHITE);
            break;
        case UNCHANGED:
            printf("%s%s -> %s%s\n", GREY, filename, new_name, WHITE);
            break;
        case UNABLE:

            break;
        default:
    }
}
