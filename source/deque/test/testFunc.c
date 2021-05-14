#include "deque.h"
#include "unity.h"

void setUp(void)
{
    ;
}
void tearDown(void)
{
    ;
}

void printInteger(void *i)
{
    printf("%d", *(int *)i);
}
void testFunc(void)
{
    C_Deque *testdeque = C_Deque_getNew(sizeof(int));
    // int a = 1;
    for (int i = 0; i < 10; ++i)
    {
        C_Deque_PushFront(testdeque, &i, true);
    }
    // for (int i = 0; i < 10; ++i)
    // {
    //     C_Deque_PushBack(testdeque, &i, true);
    // }
    for (int i = 0; i < 5; ++i)
    {
        C_Deque_PopBack(testdeque);
    }
    for (int i = 0; i < 10; ++i)
    {
        C_Deque_PushFront(testdeque, &i, true);
    }
    C_Deque_Print(testdeque, printInteger);
    for (int i = 0; i < 20; ++i)
    {
        C_Deque_PopFront(testdeque);
    }
    C_Deque_Print(testdeque, printInteger);
    C_Deque_Free(testdeque);
}