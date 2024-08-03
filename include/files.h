#ifndef FILES_H
#define FILES_H

#include <dirent.h>

#define FOUND 0
#define UNSURE 1
#define UNKNOWN 2
#define UNCHANGED 3
#define UNABLE 4

/// Finder function is applied on each filename, and, based on the outcome of
/// the search, the changes are printed on screen.
/// Function returns the number of changes made.
int analyze_filenames(
    const char *folder, DIR *dir, int finder(char *, const char *));

/// Based on search outcome, changes are printed showing both original and final
/// filename.
void print_outcome(int outcome, char *filename, char *new_name);

/// Filename is appended with a unique id if a file with the same name is found
/// in the same directory. Extension won't be added to filename.
/// Function returns 0 or 1 on whether file access succeeds.
int make_filename_unique(
    const char *dir, char *filename, const char *extension);

#endif
