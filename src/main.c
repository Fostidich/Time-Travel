//
// Created by kello on 04/04/24.
//

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "finder.h"

#define GREY "\033[0;30m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define WHITE "\033[0;37m"

#ifdef TEST
int main1(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
    if (argc == 1 || strlen(argv[0]) > 3584) {
        fprintf(stderr, "ERROR: no directory provided\n");
        exit(1);
    }

    DIR *dir;
    struct dirent *entry;
    char path[strnlen(argv[1], PATH_MAX)];
    strcpy(path, argv[1]);
    dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "ERROR: directory does not exist\n");
        exit(1);
    }

    printf("Analyzing lc_filenames in %s\n\n", path);
    long changed = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG || entry->d_name[0] == '.')
            continue;

        size_t lc_filename_length = strnlen(entry->d_name, sizeof(entry->d_name) - 1);
        char lc_filename[lc_filename_length + 16];
        strncpy(lc_filename, entry->d_name, lc_filename_length);
        lc_filename[lc_filename_length] = '\0';
        if (lc_filename_length == sizeof(entry->d_name) - 1) {
            printf("%sERROR: file name is too long -> %s...%s\n", RED, lc_filename, WHITE);
            continue;
        }

        char *first_dot = strchr(lc_filename, '.');
        size_t extension_length = 1;
        char extension[sizeof(entry->d_name) - 2];
        extension[0] = '\0';
        if (first_dot != NULL && first_dot != lc_filename + lc_filename_length - 1) {
            extension_length = lc_filename + lc_filename_length - first_dot + 1;
            strcpy(extension, first_dot);
            *first_dot = '\0';
        }

        char new_name[DATE_LEN + extension_length + 16];
        const int outcome = find_date(new_name, lc_filename);
        if (extension[0] != '\0') strcpy(first_dot, extension);
        strcpy(new_name + DATE_LEN, extension);

        if (outcome == DATE_UNKNOWN) {
            printf("%sUNKNOWN DATE: %s%s\n", RED, lc_filename, WHITE);
            continue;
        }

        if (strcmp(new_name, lc_filename) == 0) {
            printf("%s%s -> %s%s\n", GREY, lc_filename, new_name, WHITE);
            continue;
        }

        char old_path[PATH_MAX], new_path[PATH_MAX];
        snprintf(old_path, PATH_MAX, "%s/%s", path, lc_filename);
        snprintf(new_path, PATH_MAX, "%s/%s", path, new_name);
        if (access(new_path, F_OK) == 0) {
            int duplicates = 1;
            char temp[PATH_MAX];
            do {
                snprintf(temp, PATH_MAX, "%s/%.*s (%d)%s",
                         path, DATE_LEN, new_name, duplicates, extension);
                duplicates++;
            } while (access(temp, F_OK) == 0);
            strcpy(new_path, temp);
            sprintf(new_name + DATE_LEN, " (%d)%s", duplicates, extension);
        }

        if (rename(old_path, new_path) != 0) {
            printf("%sERROR: unable to rename -> %s%s\n", RED, lc_filename, WHITE);
            continue;
        }

        changed++;
        outcome == DATE_UNSURE ?
        printf("%s%s -> %s%s\n", YELLOW, lc_filename, new_name, WHITE) :
        printf("%s -> %s\n", lc_filename, new_name);
    }

    if (changed == 0) {
        fprintf(stderr, "\nERROR: no file to rename\n");
        exit(1);
    }

    printf("\n%ld files renamed\n", changed);
    closedir(dir);
    return 0;
}
