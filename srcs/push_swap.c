#include "push_swap.h"

static error_t
check_sorted(stack_t *a, stack_t *b)
{
    node_t  *head = a->head, *curr = head;
    error_t error = SORTED;
    int size = a->size;

    if (b->size) {
        MESSAGE("\n\nError stack b not empty\n");
        error = FAILURE;
    } else {
        while (size--)
        {
            if (curr->next != head
                && curr->value.index > curr->next->value.index)
            {
                MESSAGE("\n\nError stack a not sorted\n");
                MESSAGE("\tnode index '%zu' ", curr->value.index);
                MESSAGE("with node index '%zu'\n", curr->next->value.index);
                error = FAILURE;
            }
            curr = curr->next;
        }
    }
    return (error);
}

static void
put_biggest_partition_to_front(helper_t *helper)
{
    stack_t  *stack1 = helper->stack[0], *stack2 = helper->stack[1];
    int need_rotate = stack1->head->partition != helper->partition[0];

    MESSAGE("\nENTER put_biggest_partition_to_front\n");
    if (stack1->id == 'a' && need_rotate)
    {
        if (stack2->head && stack2->head->partition != helper->partition[1])
            reverse_rotate_two(helper, helper->len[0], helper->len[1]);
        else
            reverse_rotate_one(helper->stack[0], helper->len[0]);
    }
    else if (stack1->id == 'b' && need_rotate)
        reverse_rotate_one(helper->stack[0], helper->len[0]);
    MESSAGE("\tstack '%c'", stack1->id);
    MESSAGE(" biggest partition '%d'\n", helper->partition[0]);
    MESSAGE("\tstack '%c'", stack2->id);
    MESSAGE(" biggest partition '%d'\n", helper->partition[1]);
    MESSAGE("QUIT put_biggest_partition_to_front\n\n");
}

static void
push_swap(stack_t *a, stack_t *b)
{
    error_t front_sorted = FAILURE;
    helper_t    helper = {};

    while (SUCCESS == choose_partition(&helper, a, b))
    {
        if (helper.partition[0])
            put_biggest_partition_to_front(&helper);
        front_sorted = partition_is_sorted(&helper);
        if (SUCCESS == front_sorted)
            put_sorted_partiton_in_a(&helper);
        else if (helper.len[0] <= 3)
            sort_partition(&helper);
        else
            divide_partition(&helper);
    }
}

int main(int ac, char **av)
{
    stack_t *a = NULL, *b = NULL;
    error_t code = SUCCESS;

    a = calloc(1, sizeof(stack_t));
    b = calloc(1, sizeof(stack_t));
    if (NULL == a || NULL == b)
        code = ALLOCATION;
    else if (1 == ac)
        code = NUMBER_ARGUMENTS;
    else
    {
        a->id = 'a', b->id = 'b';
        init_stack_with_input(ac, av, a, &code);
        if (SUCCESS == code)
        {
            replace_values_with_sorted_index(a, &code);
            if (SUCCESS == code)
                push_swap(a, b);
            code = check_sorted(a, b);
        }
    }
    return (quit(&a, &b, code));
}
