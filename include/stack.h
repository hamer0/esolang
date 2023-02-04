#ifndef stack_h
#define stack_h

#define MAX_STACK 128  

typedef struct {
    char c;
    int index;
} StackNode;

void emptyStack();

void pushStack(char c, int index);
StackNode* popStack();

void popStackPair();

int checkStackPair(char open, char close);

#endif