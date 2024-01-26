

typedef ListNode QueueNode;
typedef List Queue;

Queue *initQueue()
{
    return (Queue *)initList();
}

void pushQueueNode(Queue *lst, void *val)
{
    addListNode1(lst, val);
}

void *popQueueNode(Queue *lst)
{
    void *rslt = (void *)(lst->head->val);
    removeListNode(lst, 0);

    return rslt;
}

int queueLength(Queue *queue)
{
    return getListLength(queue);
}

int emptyQueue(Queue *queue)
{
    return !getListLength(queue);
}

void printQueue(Queue *queue)
{
    char *seprator = "______________";
    printf("%s\n", seprator);
    //

    void *val;
    for (int i = 0; i < queueLength(queue); i++)
    {
        val = popQueueNode(queue);
        pushQueueNode(queue, val);
        printf("%p\n%s\n", val, seprator);
    }
}
