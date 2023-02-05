#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

StackNode stack[MAX_STACK];
StackNode* stackPtr = &stack[0];

void printStack() {
    StackNode* ptr;
    for(ptr = stackPtr -1; ptr >= &stack[0]; ptr--) {
        printf("\t[%c %d]\n", ptr->c, ptr->index);
    }

    printf("\n");
}

void emptyStack() {
    stackPtr = &stack[0];
}

void pushStack(char c, int index) {
    if (stackPtr == &stack[MAX_STACK]) {
        printf("Stack Overflow!\nMax stack %d exceeded!", MAX_STACK);
        exit(0);
    }

    stackPtr->c = c;
    stackPtr->index = index;
    stackPtr++;

    // printf("->");
    // printStack();
}

StackNode* popStack() {
    if (stackPtr == &stack[0]) {
        printf("Stack Underflow!\n Stack can not be popped!");
        exit(0);
    }

    // printf("<- [%c %d]\n", node->c, node->index);
    // printStack();
    stackPtr--;
    return stackPtr;
}

void popStackPair() {
    if (stackPtr - 1 == &stack[0]) {
        printf("Stack Underflow!\n Stack can not be popped!");
        exit(0);
    }

    stackPtr -= 2;
}

int checkStackPair(char open, char close) {
    return (stackPtr-1)->c == close && (stackPtr-2)->c == open;
}

int isStackEmpty() {
    return stackPtr == &stack[0];
}
