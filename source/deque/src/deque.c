#include "deque.h"

static C_DequeNode *C_DequeNode_getNewCopy(void *val, int valSize);
static C_DequeNode *C_DequeNode_getNew(void *val);
static void C_DequeNode_AssignNeighbor(C_DequeNode *cur, C_DequeNode *pre, C_DequeNode *next);

#define DequeNodeSize sizeof(C_DequeNode)
#define MemoryBlockSize 3
// static int DequeNodeValSize;

typedef struct C_DequeNode_MemoryNode
{
    char mem[DequeNodeSize];
    struct C_DequeNode_MemoryNode *next;
} C_DequeNode_MemoryNode;

typedef struct C_DequeNode_MemoryBlock
{
    C_DequeNode_MemoryNode MemoryNode[MemoryBlockSize];
    struct C_DequeNode_MemoryBlock *next;
} C_DequeNode_MemoryBlock;

static C_DequeNode_MemoryNode *MemoryNodehead = NULL;
static C_DequeNode_MemoryBlock *MemoryBlockhead = NULL;

static void AppendMemoryBlockList(void);
static C_DequeNode_MemoryBlock *C_DequeNode_MemoryBlock_getNew(void);
static void FreeMemoryFor_C_DequeNode(C_DequeNode *res);
static C_DequeNode *ApplyMemoryFor_C_DequeNode(void);

static C_DequeNode_MemoryBlock *C_DequeNode_MemoryBlock_getNew(void)
{
    C_DequeNode_MemoryBlock *res = (C_DequeNode_MemoryBlock *)malloc(sizeof(C_DequeNode_MemoryBlock));
    for (int i = 0; i < MemoryBlockSize - 1; ++i)
    {
        res->MemoryNode[i].next = &res->MemoryNode[i + 1];
    }
    res->MemoryNode[MemoryBlockSize - 1].next = NULL;
    return res;
}

static void AppendMemoryBlockList(void)
{
    C_DequeNode_MemoryBlock *tmp = C_DequeNode_MemoryBlock_getNew();
    if (MemoryBlockhead == NULL)
    {
        MemoryBlockhead = tmp;
    }
    else
    {
        C_DequeNode_MemoryBlock *Mbn = MemoryBlockhead->next;
        MemoryBlockhead->next = tmp;
        tmp->next = Mbn;
    }
    if (MemoryNodehead == NULL)
    {
        MemoryNodehead = tmp->MemoryNode;
    }
    else
    {
        C_DequeNode_MemoryNode *Mn = MemoryNodehead->next;
        tmp->MemoryNode[MemoryBlockSize - 1].next = Mn;
        MemoryNodehead->next = &tmp->MemoryNode[0];
    }
}

static C_DequeNode *ApplyMemoryFor_C_DequeNode(void)
{
    if (MemoryNodehead == NULL)
    {
        AppendMemoryBlockList();
    }
    C_DequeNode *res = (C_DequeNode *)MemoryNodehead->mem;
    MemoryNodehead = MemoryNodehead->next;
    return res;
}

static void FreeMemoryFor_C_DequeNode(C_DequeNode *res)
{
    if (MemoryNodehead == NULL)
    {
        MemoryNodehead = (C_DequeNode_MemoryNode *)res;
        MemoryNodehead->next = NULL;
    }
    else
    {
        C_DequeNode_MemoryNode *tmp = (C_DequeNode_MemoryNode *)res;
        tmp->next = MemoryNodehead;
        MemoryNodehead = tmp;
    }
}

C_Deque *C_Deque_getNew(size_t valSize)
{
    C_Deque *res = (C_Deque *)malloc(sizeof(C_Deque));
    res->head = NULL;
    res->end = NULL;
    res->valSize = valSize;
    return res;
}

void C_Deque_Free(C_Deque *d)
{
    repoDequeVolatile(d, dn)
    {
        C_DequeNode *nex = dn->next;
        free(dn->val);
        // free(dn);
        FreeMemoryFor_C_DequeNode(dn);
        dn = nex;
    }
    free(d);
}

void C_Deque_Print(C_Deque *d, void (*print)(void *))
{
    repoDeque(d, node)
    {
        print(node->val);
        if (node == d->end)
        {
            putchar('\n');
        }
        else
        {
            putchar('\t');
        }
    }
}

//will copy
static C_DequeNode *C_DequeNode_getNewCopy(void *val, int valSize)
{
    // C_DequeNode *res = (C_DequeNode *)malloc(sizeof(C_DequeNode));
    C_DequeNode *res = ApplyMemoryFor_C_DequeNode();
    res->next = NULL;
    res->pre = NULL;
    res->val = malloc(valSize);
    memcpy(res->val, val, valSize);
    return res;
}
//won't copy
static C_DequeNode *C_DequeNode_getNew(void *val)
{
    // C_DequeNode *res = (C_DequeNode *)malloc(sizeof(C_DequeNode));
    C_DequeNode *res = ApplyMemoryFor_C_DequeNode();
    res->pre = NULL;
    res->next = NULL;
    res->val = val;
    return res;
}
static void C_DequeNode_AssignNeighbor(C_DequeNode *cur, C_DequeNode *pre, C_DequeNode *next)
{
    cur->next = next;
    cur->pre = pre;
}

//this function will copy value  where val points to
void C_Deque_PushFront(C_Deque *d, void *val, bool toCopy)
{
    if (d->head == NULL)
    {
        if (toCopy)
        {
            d->head = C_DequeNode_getNewCopy(val, d->valSize);
        }
        else
        {
            d->head = C_DequeNode_getNew(val);
        }
        d->end = d->head;
        C_DequeNode_AssignNeighbor(d->head, NULL, NULL);
    }
    else
    {
        C_DequeNode *newHead;
        if (toCopy)
        {
            newHead = C_DequeNode_getNewCopy(val, d->valSize);
        }
        else
        {
            newHead = C_DequeNode_getNew(val);
        }
        C_DequeNode_AssignNeighbor(newHead, NULL, d->head);
        C_DequeNode_AssignNeighbor(d->head, newHead, d->head->next);
        d->head = newHead;
    }
}
void C_Deque_PushBack(C_Deque *d, void *val, bool toCopy)
{
    if (d->end == NULL)
    {
        if (toCopy)
        {
            d->end = C_DequeNode_getNewCopy(val, d->valSize);
        }
        else
        {
            d->end = C_DequeNode_getNew(val);
        }
        d->head = d->end;
        C_DequeNode_AssignNeighbor(d->end, NULL, NULL);
    }
    else
    {
        C_DequeNode *newEnd;
        if (toCopy)
        {
            newEnd = C_DequeNode_getNewCopy(val, d->valSize);
        }
        else
        {
            newEnd = C_DequeNode_getNew(val);
        }
        C_DequeNode_AssignNeighbor(newEnd, d->end, NULL);
        C_DequeNode_AssignNeighbor(d->end, d->end->pre, newEnd);
        d->end = newEnd;
    }
}

bool C_Deque_PopFront(C_Deque *d)
{
    if (d->head == NULL)
    {
        return false;
    }
    C_DequeNode *tmp = d->head;
    d->head = tmp->next;
    if (d->head != NULL)
    {
        d->head->pre = NULL;
    }
    free(tmp->val);
    // free(tmp);
    FreeMemoryFor_C_DequeNode(tmp);
    return true;
}

bool C_Deque_PopBack(C_Deque *d)
{
    if (d->end == NULL)
    {
        return false;
    }
    C_DequeNode *tmp = d->end;
    d->end = tmp->pre;
    if (d->end != NULL)
    {
        d->end->next = NULL;
    }
    free(tmp->val);
    // free(tmp);
    FreeMemoryFor_C_DequeNode(tmp);
    return true;
}