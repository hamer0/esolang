#include <stdlib.h>
#include <string.h>

// Check if string ends with suffix
int strendsw(const char *str, const char *suffix) {
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

// Get string file extension
char* strext(const char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot;
}

char* chartos(char character) {
    char* string = calloc(2, sizeof(char));
    string[0] = character;
    string[1] = '\0';

    return string;
}