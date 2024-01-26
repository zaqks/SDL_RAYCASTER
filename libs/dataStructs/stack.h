

typedef ListNode StackNode;
typedef List Stack;

Stack *initStack()
{
    return (Stack *)initList();
}

int stackLength(Stack *stk)
{
    return getListLength(stk);
}

int emptyStack(Stack *stk)
{
    return !getListLength(stk);
}

void pushStackNode(Stack *stk, void *val)
{
    addListNode2(stk, val);
}

void *popStackNode(Stack *stk)
{
    void *rslt = (void *)(stk->head->val);
    removeListNode(stk, 0);

    return rslt;
}

void printStack(Stack *stk)
{
    printf("__\n");
    //
    Stack *tmp = initStack();
    void *val;

    while (!emptyStack(stk))
    {
        val = popStackNode(stk);
        pushStackNode(tmp, val);
        printf("%p\n__\n", val);
    }
    // refill
    while (!emptyStack(tmp))
    {
        pushStackNode(stk, popStackNode(tmp));
    }

    free(tmp);
}
