#ifndef main_h
#define main_h

#include "../include/pairs.h"

#define EXT ".zxb1"
#define EXE "zxb1"

char nextChar();
void readUntilChar(char match);
void shiftWindow(int* index, char** byte, int direction);
void windowOperator(char operator);
int getWindowValue();
SkipPairList pairMatchingChars(char open, char close);

#endif