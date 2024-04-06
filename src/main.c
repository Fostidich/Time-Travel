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

int main(int argc, char **argv) {
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

    printf("Analyzing filenames in %s\n\n", path);
    long changed = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG || entry->d_name[0] == '.')
            continue;

        size_t filename_length = strnlen(entry->d_name, sizeof(entry->d_name) - 1);
        char filename[filename_length + 16];
        strncpy(filename, entry->d_name, filename_length);
        filename[filename_length] = '\0';
        if (filename_length == sizeof(entry->d_name) - 1) {
            printf("%sERROR: file name is too long -> %s...%s\n", RED, filename, WHITE);
            continue;
        }

        char *first_dot = strchr(filename, '.');
        size_t extension_length = 1;
        char extension[sizeof(entry->d_name) - 2];
        int has_extension = first_dot != NULL && first_dot != filename + filename_length - 1;
        if (has_extension) {
            extension_length = filename + filename_length - first_dot + 1;
            strcpy(extension, first_dot);
            *first_dot = '\0';
        }

        char new_name[DATE_LEN + extension_length + 16];
        const int outcome = findDate(new_name, filename);
        if (has_extension) {
            strcpy(first_dot, extension);
            strcpy(new_name + DATE_LEN, extension);
        }

        if (outcome == DATE_UNKNOWN) {
            printf("%sUNKNOWN DATE: %s%s\n", RED, filename, WHITE);
            continue;
        }

        if (strcmp(new_name, filename) == 0) {
            printf("%s%s -> %s%s\n", GREY, filename, new_name, WHITE);
            continue;
        }

        char old_path[PATH_MAX], new_path[PATH_MAX];
        snprintf(old_path, PATH_MAX, "%s/%s", path, filename);
        snprintf(new_path, PATH_MAX, "%s/%s", path, new_name);
        if (access(new_path, F_OK) == 0) {
            int duplicates = 1;
            char temp[PATH_MAX];
            do {
                if (has_extension) {
                    char *last_dot = strrchr(new_path, '.');
                    snprintf(temp, PATH_MAX, "%.*s (%d)%s",
                             (int) (last_dot - new_path), new_path, duplicates, extension);
                } else {
                    snprintf(temp, PATH_MAX, "%.4000s (%d)", new_path, duplicates);
                }
                duplicates++;
            } while (access(temp, F_OK) == 0);
            strcpy(new_path, temp);
            has_extension ?
            sprintf(new_name + DATE_LEN, " (%d)%s", duplicates, extension) :
            sprintf(new_name + DATE_LEN, " (%d)", duplicates);
        }

        if (rename(old_path, new_path) != 0) {
            printf("%sERROR: unable to rename -> %s%s\n", RED, filename, WHITE);
            continue;
        }

        changed++;
        outcome == DATE_UNSURE ?
        printf("%s%s -> %s%s\n", YELLOW, filename, new_name, WHITE) :
        printf("%s -> %s\n", filename, new_name);
    }

    if (changed == 0) {
        fprintf(stderr, "\nERROR: no file to rename\n");
        exit(1);
    }

    printf("\n%ld files renamed\n", changed);
    closedir(dir);
    return 0;
}
