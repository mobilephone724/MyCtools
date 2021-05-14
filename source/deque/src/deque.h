#ifndef C_Deque_H
#define C_Deque_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct C_DequeNode
{
    /* data */
    void *val;
    struct C_DequeNode *next;
    struct C_DequeNode *pre;
} C_DequeNode;

// typedef C_DequeNode *C_DequeNodePtr;

typedef struct C_Deque
{
    C_DequeNode *head;
    C_DequeNode *end;
    size_t valSize;
} C_Deque;

#define repoDeque(d, i) for (C_DequeNode *i = d->head; i != NULL; i = i->next)
#define repoDequeVolatile(d, i) for (C_DequeNode *i = d->head; i != NULL;)

C_Deque *C_Deque_getNew(size_t valSize);
void C_Deque_Free(C_Deque *d);

void C_Deque_Print(C_Deque *d, void (*print)(void *));

//this function will copy value  where val points to
void C_Deque_PushFront(C_Deque *d, void *val, bool toCopy);
void C_Deque_PushBack(C_Deque *d, void *val, bool toCopy);

//this function will not copy value  where val points to
//so be sure that the value is on heap
// void C_Deque_PushFront(C_Deque *d, void **val);
// void C_Deque_PushBack(C_Deque *d, void **val);

bool C_Deque_PopFront(C_Deque *d);
bool C_Deque_PopBack(C_Deque *d);

#endif