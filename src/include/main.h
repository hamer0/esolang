#ifndef main_h
#define main_h

#define EXT ".zxb1"
#define EXE "zxb1"

void readUntilChar(char match, FILE *fp);
void shiftWindow(int* index, char** byte, int direction);
void windowOperator(char operator);
int getWindowValue();

#endif