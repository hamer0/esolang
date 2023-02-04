#ifndef pairs_h
#define pairs_h

#define DEFAULT_LIST_CAP 8

typedef struct {
    int start;
    int end;
} SkipPair;

typedef struct {
    SkipPair* pairs;
    int size;
    int cap;
} SkipPairList;

SkipPairList skipPairList_create();
void skipPairList_add(SkipPairList* list, SkipPair pair);

#endif