#ifndef main_h
#define main_h

#define EXT ".zxb1"
#define EXE "zxb1"

void readUntilChar(char match, FILE *fp);
void incrementWindow(int* index, char** byte);
void decrementWindow(int* index, char** byte);
void windowOperator(char operator);
int getWindowValue();

#endif