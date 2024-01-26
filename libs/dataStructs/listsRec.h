//recursivity support for lists


typedef struct listNode
{
    void *val;
    struct listNode *next;
} ListNode;

typedef struct
{
    ListNode *head;
} List;

int getListLengthRec(ListNode *head, int indx)
{
    if (!head)
    {
        return indx;
    }
    getListLengthRec(head->next, indx + 1);
}

ListNode *getListQueueRec(ListNode *head)
{
    if (!head->next)
    {
        return head;
    }
    getListQueueRec(head->next);
}

void printListRec(ListNode *head)
{
    if (head)
    {
        printf("%p ", head->val);
        printListRec(head->next);
    }
}