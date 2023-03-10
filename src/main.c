#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "../include/main.h"
#include "../include/strutil.h"
#include "../include/stack.h"
#include "../include/pairs.h"

#define byteMask(from, to) ((0xff >> from) & ~(0xff >> (to + 1)))   // mask of 1s between given value

FILE* fp;

char* wlb;
char* wrb;

int wli = 0;
int wri = 0;

char STATUS;  // byte for flagging status. [ x | x | x | x | x | x | x | ERROR ] 

SkipPairList comments;
SkipPairList blocks;
SkipPairList loopBlocks;

int main(int argc, char *argv[]) {
    if (argc < 2) { // Validate against too few args
        printf("\n\nToo few arguments!\nUsage: %s <filepath>", EXE);
        return EXIT_FAILURE;
    }

    if (argc > 2) { // Validate against too many args
        printf("\n\nToo many arguments!\nUsage: %s <filepath>", EXE);
        return EXIT_FAILURE;
    }

    if (!strendsw(argv[1], EXT)) { // Validate against incorrect file extension
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

    char memoryBlock[] = {0x00, 0x00, 0x00, 0x00};
    wlb = &memoryBlock[0];
    wrb = &memoryBlock[0];

    fp = fopen(argv[1], "r");

    // Preprocess step
    comments = pairMatchingChars('"', '"');     // Get comments first as they may impact other pairs e.g. if a bracket is commented out
    blocks = pairMatchingChars('{', '}');
    loopBlocks = pairMatchingChars('(', ')');

    if (STATUS &= STATUS_ERROR) exit(EXIT_FAILURE);

    // Interpret step
    for (char c = nextChar(); c != EOF; c = nextChar())
    {
        if (isspace(c)) continue;
        if (c == '{' || c == '(') continue;

        switch (c) {
            case '>':
                shiftWindow(&wli, &wlb, 1);
                shiftWindow(&wri, &wrb, 1);
                break;
            case '<':
                shiftWindow(&wli, &wlb, -1);
                shiftWindow(&wri, &wrb, -1);
                break;
            case '+':
                shiftWindow(&wri, &wrb, 1);
                break;
            case '-':
                shiftWindow(&wri, &wrb, -1);
                break;
            case '1':
            case '0':
            case '~':
                windowOperator(c);
                break;
            case '@':
                printf("%c", getWindowValue());
                break;
            case '}':
                break;
            case ')':
                jumpBack(loopBlocks, ftell(fp) - 1);
                break;
            default:
                printf("Syntax Error!\nUnexpected symbol '%c' (%d)", c, c);
                return EXIT_FAILURE;
        }
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

char nextChar() {
    for (int i=0; i<comments.size; i++) {
        if (ftell(fp) == comments.pairs[i].start) {
            fseek(fp, comments.pairs[i].end + 1, SEEK_SET);
            break;
        }
    }

    return fgetc(fp);
}

void shiftWindow(int* index, char** byte, int direction) {
    *index = (*index + direction) & 7;                              // shift index value (& 7 wraps if value exceeds 7)
    if (*index == 0 && direction > 0) (*byte)++;                    // if index is 0 (has been wrapped) then increment byte pointer
    if (*index == 7 && direction < 0) (*byte)--;                    // if index is 7 (has been wrapped) then increment byte pointer
}

void windowOperator(char operator) {
    int from, to;
    for(char* ptr = wlb; ptr <= wrb; ptr++) {                       // iterate for each byte between left and right window pointers
        from = 0;                                                   // default from/to is 0/7
        to = 7;

        if (ptr == wlb) from = wli;                                 // if byte is left pointer from is the left pointer index
        if (ptr == wrb) to = wri;                                   // if byte is right pointer (can be both) to is right pointer index

        switch(operator) {                                          // perform operation on byte between from/to bits
            case '1': *ptr |= byteMask(from, to); break;            // set bits
            case '0': *ptr &= ~byteMask(from, to); break;           // clear bits
            case '~': *ptr ^= byteMask(from, to); break;            // flip bits
            default: break;
        }
    }
}

int getWindowValue() {
    int val = (*wlb & byteMask(wli, 7));                            // Get value of the bits in the left byte from the index

    for(char* ptr = wlb + 1; ptr <= wrb; ptr++) {                   // iterate for each byte upto window right pointer
        val << 8;                                                   // shift 1 byte left
        val += *ptr;                                                // add the byte to the value
    }

    return val >> (7 - wri);                                        // shift right to remove excess upto index in right window byte
}

SkipPairList pairMatchingChars(char open, char close) { // saves the indexes of matching chars given the open and close char e.g. {} () ""
    SkipPairList list = skipPairList_create();
    emptyStack();

    for (char c = fgetc(fp); c != EOF; c = fgetc(fp)) {
        if (c == open || c == close) pushStack(c, ftell(fp) - 1);
        if(checkStackPair(open, close)) {
            StackNode* closeNode = popStack();
            StackNode* openNode = popStack();
            skipPairList_add(&list, (SkipPair){openNode->index, closeNode->index});
        }
    }

    while (!isStackEmpty()) {
        StackNode* node = popStack();
        printf("Syntax Error!\n'%c' is not closed!\n", node->c);
        STATUS |= STATUS_ERROR;
    }

    fseek(fp, 0, SEEK_SET);
    return list;
}

void jumpBack(SkipPairList list, int from) {    
    for(int i=0; i< list.size; i++) {
        if (list.pairs[i].end == from) {
            fseek(fp, list.pairs[i].start, SEEK_SET);
            return;
        }
    }

    printf("Jump Error!\nNo jump back could be found for %d in list", from);
    exit(EXIT_FAILURE);
}