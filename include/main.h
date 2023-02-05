#ifndef main_h
#define main_h

#include "../include/pairs.h"

#define EXT ".zxb1"
#define EXE "zxb1"

#define STATUS_ERROR 0x01

char nextChar();
void readUntilChar(char match);
void shiftWindow(int* index, char** byte, int direction);
void windowOperator(char operator);
int getWindowValue();
SkipPairList pairMatchingChars(char open, char close);
void jumpBack(SkipPairList list, int from);

#endif