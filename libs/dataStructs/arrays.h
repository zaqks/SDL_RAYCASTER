typedef struct
{
    void *val;
} ArrayNode;

typedef struct
{
    ArrayNode *arr; // points the the first elem
    int length;
} Array;

Array *initArray()
{
    Array *arr = (Array *)malloc(sizeof(Array));
    arr->length = 0;
    arr->arr = NULL;
    return arr;
}

void pushArrayNode(Array *arr, void *val)
{
    arr->length += 1;

    if (arr->arr == NULL)
    {
        arr->arr = malloc(sizeof(ArrayNode));
    }
    else

    {
        arr->arr = realloc(arr->arr, arr->length * sizeof(ArrayNode));
    }

    ArrayNode new;
    new.val = val;

    arr->arr[arr->length - 1] = new;
}

void delArrayNode(Array *arr, int indx)
{
    for (int i = indx; i < arr->length - 1; i++)
    {
        arr->arr[i] = arr->arr[i + 1];
    }

    arr->length -= 1;
    arr->arr = realloc(arr->arr, (arr->length) * sizeof(ArrayNode));
}

void printArray(Array *arr)
{
    printf("[");

    for (int i = 0; i < arr->length; i++)
    {
        printf("%p", arr->arr[i].val);
        if (i < arr->length - 1)
        {
            printf(", ");
        }
    }

    printf("]\n");
}

void freeArray(Array *arr)
{
    int length = arr->length;
    for (int i = 0; i < length; i++)
    {
        free(arr->arr[i].val);
    }
    free(arr->arr);
    free(arr);
}