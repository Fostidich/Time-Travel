#include <stdlib.h>
#include <string.h>

#include "finder.h"

/// Argv contains, in order, input filename string, expected found string,
/// outcome int value
int main(int argc, char **argv) {
    char result[256];
    int outcome = find_date(result, argv[0]);
    return !(strcmp(argv[1], result) == 0 && outcome == atoi(argv[2]));
}
