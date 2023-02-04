#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "./include/main.h"
#include "./include/strutil.h"

#define MAX_STACK 255                                               // max stack is 255                                      
#define DEFAULT_LIST_CAP 8
#define byteMask(from, to) ((0xff >> from) & ~(0xff >> (to + 1)))   // mask of 1s between given value

StackNode stack[MAX_STACK];
StackNode* stackPtr = &stack[0];

SkipPairList comments;
SkipPairList blocks;
SkipPairList loopBlocks;


void pushStack(char c, int index) {
    if (stackPtr == &stack[MAX_STACK]) {
        printf("Stack Overflow!\nMax stack %d exceeded!", MAX_STACK);
        exit(EXIT_FAILURE);
    }

    stackPtr->c = c;
    stackPtr->index = index;
    stackPtr++;
}

void popStackPair() {
    if (stackPtr - 1 == &stack[0]) {
        printf("Stack Underflow!\n Stack can not be popped!");
        exit(EXIT_FAILURE);
    }

    stackPtr -= 2;
}

void skipPairList_add(SkipPairList* list, SkipPair pair) {
    if (list->size == list->cap) {
        int new_capacity = list->cap * 2;
        SkipPair* new_pairs = realloc(list->pairs, sizeof(SkipPair) * new_capacity);

        if (new_pairs == NULL) {
            printf("Memory Error: Failed to resize the skip pair list\n");
            exit(EXIT_FAILURE);
        }

        list->pairs = new_pairs;
        list->cap = new_capacity;
    }

    list->pairs[list->size++] = pair;
}

SkipPairList pairMatchingChars(char open, char close, FILE* fp) { // saves the indexes of matching chars given the open and close char e.g. {} () ""
    fseek(fp, 0, SEEK_SET);
    char c;

    SkipPairList list;
    list.pairs = malloc(sizeof(SkipPair) * DEFAULT_LIST_CAP);
    list.size = 0;
    list.cap = DEFAULT_LIST_CAP;

    for (c = fgetc(fp); c != EOF; c = fgetc(fp)) { // strange for loop lol
        if (c == open || c == close) pushStack(c, ftell(fp) - 1);
        if((stackPtr-1)->c == close && (stackPtr-2)->c == open) {
            skipPairList_add(&list, (SkipPair){(stackPtr-2)->index, (stackPtr-1)->index});
            popStackPair();
        }
    }

    return list;
}

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

    // Preprocess step
    comments = pairMatchingChars('"', '"', fp);     // Get comments first as they may impact other pairs e.g. if a bracket is commented out
    blocks = pairMatchingChars('{', '}', fp);
    loopBlocks = pairMatchingChars('(', ')', fp);
    
    // Interpret step
    fseek(fp, 0, SEEK_SET);

    while (1)
    {
        c = fgetc(fp);

        if (c == '\0' || c == EOF) break;
        if (isspace(c)) continue;

        switch (c) {
            case '"':
                readUntilChar('"', fp);
                break;
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