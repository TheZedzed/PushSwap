#include "push_swap.h"

static const char  *g_errors[5] = {
    "ALLOCATION", "NUMBER_ARGUMENTS", "BAD_NUMBER", "DUPLICATE", NULL
};

static void
free_stack(stack_t **el)
{
    node_t *head = (*el)->head, *curr = NULL;
    size_t  size = (*el)->size;

    while (size--)
    {
        curr = head;
        head = head->next;
        free(curr);
    }
    free(*el);
}

void
set_node_sorted(stack_t *stack, int len)
{
    node_t  *head = stack->head;

    while (len--)
    {
        head->partition = -1;
        MESSAGE("\tnode index '%zu'", head->value.index);
        MESSAGE(" in stack '%c' sorted\n", stack->id);
        head = head->next;
    }
}

void
rotate_one(stack_t *stack, int len)
{
    MESSAGE("\nENTER rotate_one\n");
    if (stack->id == 'a') {
        while (len)
        {
            operator(RA, stack, NULL);
            --len;
        }
    } else {
        while (len)
        {
            operator(RB, stack, NULL);
            --len;
        }
    }
    MESSAGE("QUIT rotate_one\n\n");
}

void
reverse_rotate_one(stack_t *stack, int len)
{
    MESSAGE("\nENTER reverse_rotate_one\n");
    if (stack->id == 'a') {
        while (len)
        {
            operator(RRA, stack, NULL);
            --len;
        }
    } else {
        while (len)
        {
            operator(RRB, stack, NULL);
            --len;
        }
    }
    MESSAGE("QUIT reverse_rotate_one\n\n");
}

void
reverse_rotate_two(helper_t *helper, int tg_nodes, int sg_nodes)
{
    MESSAGE("\nENTER reverse_rotate_two\n");
    while (tg_nodes && sg_nodes)
    {
        operator(RRR, helper->stack[0], helper->stack[1]);
        --tg_nodes;
        --sg_nodes;
    }
    reverse_rotate_one(helper->stack[0], tg_nodes);
    reverse_rotate_one(helper->stack[1], sg_nodes);
    MESSAGE("QUIT reverse_rotate_two\n\n");
}

int
quit(stack_t **a, stack_t **b, int error)
{
    const char* error_description = NULL;

    if (NULL != a && NULL != *a) free_stack(a);
    if (NULL != b && NULL != *b) free_stack(b);

    if (1 < error)
    {
        error_description = g_errors[error - 2];
        write(STDERR_FILENO, "Error : ", 7);
        write(STDERR_FILENO, error_description, strlen(error_description));
        write(STDERR_FILENO, "\n", 1);
    }
    else
        write(STDERR_FILENO, "Sorted =D\n", 10);
    exit(1 < error);
}
