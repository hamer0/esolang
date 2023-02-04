#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "./include/main.h"
#include "./include/strutil.h"

char* wlb;
char* wrb;

int wli = 0;
int wri = 0;

int main(int argc, char *argv[]) {


    // Validate against too few args
    if (argc < 2) {
        printf("\n\nToo few arguments!\nUsage: %s <filepath>", EXE);
        return EXIT_FAILURE;
    }

    // Validate against too many args
    // (Will have to remove later if flags are added)
    if (argc > 2) {
        printf("\n\nToo many arguments!\nUsage: %s <filepath>", EXE);
        return EXIT_FAILURE;
    }

    /*
    *   Validate against incorrect file extension
    */
    if (!strendsw(argv[1], EXT)) {
        printf("\n\nArgument Error!\nFile must be a '%s' file. Got a '%s' file instead!", EXT, strext(argv[1]));
        return EXIT_FAILURE;
    }

    /*
    *   Validate against file doesn't exist
    *   https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
    */
    struct stat buffer;   
    if(stat(argv[1], &buffer) != 0) {
        printf("\n\nFile Error!\nSpecified file '%s' does not exist or otherwise cannot be found!", argv[1]);
        return EXIT_FAILURE;
    }

    FILE *fp;
    char c;
    int depth = 0;

    char memoryBlock[] = {0x00, 0x00, 0x00, 0x00};

    wlb = &memoryBlock[0];
    wrb = &memoryBlock[0];

    fp = fopen(argv[1], "r");

    while (1)
    {
        c = fgetc(fp);

        if (c == '\0' || c == EOF) break;
        if (isspace(c)) continue;

        switch (c) {
            case '"':
                readUntilChar('"', fp);
                break;
            case '+':
                incrementWindow(&wli, &wlb);
                incrementWindow(&wri, &wrb);
                break;
            case '-':
                decrementWindow(&wli, &wlb);
                decrementWindow(&wri, &wrb);
                break;
            case '>':
                incrementWindow(&wri, &wrb);
                break;
            case '<':
                decrementWindow(&wri, &wrb);
                break;
            case '/':
                setOrClearWindow(1);
                break;
            case '\\':
                setOrClearWindow(0);
                break;
            case '@':
                printf("%c", getWindowValue());
                break;
            default:
                printf("\nUnrecognised character '%c' (%d)", c, c);
                return EXIT_FAILURE;
        }
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

void readUntilChar(char match, FILE *fp) {
    char current;
    while(current = fgetc(fp) != match) {
        if (feof(fp)) {
            printf("\n\nEOF Reached!\nExpected to read a %c", match);
            exit(EXIT_FAILURE);
        }
    }
}

void incrementWindow(int* index, char** byte) {
    *index = (*index + 1) % 8;
    if (*index == 0) (*byte)++;
}

void decrementWindow(int* index, char** byte) {
    (*index)--;
    if (*index < 0) {
        *index = 7;
        (*byte)--;
    }
}

void setOrClearWindow(int set) { // Set/Clear from window wlb[wli] : wrb[wri]
    int from, to;
    for(char* ptr = wlb; ptr <= wrb; ptr++) {
        from = 0;
        to = 7;

        if (ptr == wlb) from = wli;
        if (ptr == wrb) to = wri;

        set ? setBits(ptr, from, to) : clearBits(ptr, from, to);
    }
}

int getWindowValue() {
    int val = (*wlb & ~byteMask(0, wli));

    for(char* ptr = wlb + 1; ptr <= wrb; ptr++) {
        val << 8;
        val += *ptr;
    }

    return val >> (7 - wri);
}

void setBits(char* byte, int from, int to) {
    *byte |= byteMask(from, to);
}

void clearBits(char* byte, int from, int to) {
    *byte &= ~byteMask(from, to);
}

char byteMask(int from, int to) {   //Mask of 1s between indexes given
    return (0xff >> from) & ~(0xff >> (to + 1));
}