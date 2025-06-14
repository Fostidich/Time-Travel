#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "files.h"
#include "finder.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "ERROR: no directory provided\n");
        return 1;
    }

    if (strnlen(argv[1], PATH_MAX) == PATH_MAX) {
        fprintf(stderr, "ERROR: directory name is too long\n");
        return 2;
    }

    DIR *dir = opendir(argv[1]);
    if (!dir) {
        fprintf(stderr, "ERROR: directory not found\n");
        return 3;
    }

    printf("Analyzing filenames in %s\n\n", argv[1]);
    int changes = analyze_filenames(argv[1], dir, find_date);
    printf("\n%d files renamed\n", changes);

    closedir(dir);
    return 0;
}
