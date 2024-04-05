//
// Created by kello on 04/04/24.
//

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "finder.h"

int main(int argc, char **argv) {
    if (argc == 1) {
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

    long changed = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG)
            continue;

        size_t filename_length = strnlen(entry->d_name, sizeof(entry->d_name) - 1);
        char filename[filename_length + 1];
        strncpy(filename, entry->d_name, filename_length);
        filename[filename_length] = '\0';
        if (filename_length == sizeof(entry->d_name) - 1) {
            printf("\\033[0;31mERROR: file name is too long -> %s...\\033[0m\n", filename);
            continue;
        }

        char new_name[FILENAME_LEN];
        const int outcome = findDate(new_name, filename);
        if (outcome == DATE_UNKNOWN) {
            printf("\\033[38;5;208mUNKNOWN DATE -> %s\\033[0m\n", filename);
            continue;
        } else if (outcome == DATE_UNSURE) {
            printf("\\033[38;5;226mUNSURE DATE -> %s\\033[0m\n", filename);
        }

        if (strcmp(new_name, filename) == 0) {
            printf("\\033[38;5;242m%s -> %s\\033[0m\n", filename, new_name);
            continue;
        }

        char old_path[PATH_MAX], new_path[PATH_MAX];
        snprintf(old_path, PATH_MAX, "%s/%s", path, filename);
        snprintf(new_path, PATH_MAX, "%s/%s", path, new_name);
        if (rename(old_path, new_path) != 0) {
            printf("\\033[0;31mERROR: unable to rename -> %s\\033[0m\n", filename);
            continue;
        }

        changed++;
        printf("%s -> %s\n", filename, new_name);
    }

    if (changed == 0) {
        fprintf(stderr, "ERROR: no file to rename\n");
        exit(1);
    }

    printf("%ld files renamed\n", changed);
    closedir(dir);
    return 0;
}


// TODO: check if path is absolute or relative
