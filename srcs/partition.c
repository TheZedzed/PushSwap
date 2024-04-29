#include "push_swap.h"

static void
biggest_partition(stack_t *stack, size_t *len, size_t *min, int *part)
{
    node_t  *head = stack->head, *curr = NULL;
    int rr = head && head->partition < head->prev->partition;
    size_t size = stack->size;

    curr = rr ? head->prev : head;
    *part = curr->partition;
    *min = curr->value.index;
    MESSAGE("\tbiggest partition with rr '%d' part '%d' and min '%zu'\n", rr, *part, *min);
    if (rr) {
        while (*len < size && curr->partition == *part)
        {
            ++(*len);
            if (curr->value.index < *min)
                *min = curr->value.index;
            curr = curr->prev;
        }
    } else {
        while (*len < size && curr->partition == *part)
        {
            ++(*len);
            if (curr->value.index < *min)
                *min = curr->value.index;
            curr = curr->next;
        }
    }
    MESSAGE("\tIn stack '%c'", stack->id);
    MESSAGE(" biggest partition '%d' with len '%zu'", *part, *len);
    MESSAGE(" and min '%zu'\n", *min);
}

void
put_sorted_partiton_in_a(helper_t *helper)
{
    int len = helper->len[0];

    MESSAGE("\nENTER put_sorted_partiton_in_a\n");
    if (helper->stack[0]->id == 'b')
    {
        while (len--)
            operator(PA, helper->stack[0], helper->stack[1]);
        set_node_sorted(helper->stack[1], helper->len[0]);
    }
    else
        set_node_sorted(helper->stack[0], len);
    MESSAGE("QUIT put_sorted_partiton_in_a\n\n");
}

error_t
partition_is_sorted(helper_t *helper)
{
    node_t  *head = helper->stack[0]->head;
    int dir = helper->stack[0]->id == 'a' ? 0 : 1;
    int len = helper->len[0];

    if (1 == dir) {
        while (len--)
        {
            if (head->value.index > head->next->value.index)
                head = head->next;
            else
                return (FAILURE);
        }
    } else {
        while (len--)
        {
            if (head->value.index < head->next->value.index)
                head = head->next;
            else
                return (FAILURE);
        }
    }
    return (SUCCESS);
}

error_t
choose_partition(helper_t *helper, stack_t *a, stack_t *b)
{
    size_t  a_len = 0, b_len = 0, a_min = 0, b_min = 0;
    int a_part = -1, b_part = -1, biggest_in_a = 0;

    MESSAGE("\nENTER choose_partition\n");
    if (a->head)
        biggest_partition(a, &a_len, &a_min, &a_part);
    if (b->head)
        biggest_partition(b, &b_len, &b_min, &b_part);
    if (a_part != -1 || b_part != -1)
    {
        biggest_in_a = a_part > b_part;
        helper->min = biggest_in_a ? a_min : b_min;
        helper->len[0] = biggest_in_a ? a_len : b_len;
        helper->len[1] = biggest_in_a ? b_len : a_len;
        helper->partition[0] = biggest_in_a ? a_part : b_part;
        helper->partition[1] = biggest_in_a ? b_part : a_part;
        helper->stack[0] = biggest_in_a ? a : b;
        helper->stack[1] = biggest_in_a ? b : a;
        MESSAGE("QUIT choose_partition \'%d\' ", helper->partition[0]);
        MESSAGE("with a len of \'%zu\' ", helper->len[0]);
        MESSAGE("in the stack \'%c\'\n\n", helper->stack[0]->id);
        return (SUCCESS);
    }
    MESSAGE("QUIT choose_partition and STOP\n\n");
    return (FAILURE);
}
