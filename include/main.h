#ifndef main_h
#define main_h

#define EXT ".zxb1"
#define EXE "zxb1"

char nextChar();
void readUntilChar(char match);
void shiftWindow(int* index, char** byte, int direction);
void windowOperator(char operator);
int getWindowValue();

// typedef struct {
//     char c;
//     int index;
// } StackNode;

typedef struct {
    int start;
    int end;
} SkipPair;

typedef struct {
    SkipPair* pairs;
    int size;
    int cap;
} SkipPairList;

#endif