#ifndef main_h
#define main_h

#define EXT ".zxb1"
#define EXE "zxb1"

void readUntilChar(char match, FILE *fp);
void incrementWindow(int* index, char** byte);
void decrementWindow(int* index, char** byte);
void setOrClearWindow(int set);
void charOut();
void printWindow();
void printWindowBytes();

void setBits(char* byte, int from, int to);
void clearBits(char* byte, int from, int to);

char byteMask(int from, int to);


#endif