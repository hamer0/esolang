#include <stdlib.h>
#include <stdio.h>

#include "../include/pairs.h"

SkipPairList skipPairList_create() {
    SkipPairList list;
    list.pairs = malloc(sizeof(SkipPair) * DEFAULT_LIST_CAP);
    list.size = 0;
    list.cap = DEFAULT_LIST_CAP;

    return list;
}

void skipPairList_add(SkipPairList* list, SkipPair pair) {
    if (list->size == list->cap) {
        int new_capacity = list->cap * 2;
        SkipPair* new_pairs = realloc(list->pairs, sizeof(SkipPair) * new_capacity);

        if (!new_pairs) {
            printf("Memory Error: Failed to resize the skip pair list\n");
            exit(0);
        }

        list->pairs = new_pairs;
        list->cap = new_capacity;
    }

    list->pairs[list->size++] = pair;
}