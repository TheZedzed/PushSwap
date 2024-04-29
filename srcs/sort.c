#include "push_swap.h"

static size_t
init_three(stack_t *stack, size_t *first, size_t *second, size_t *third)
{
    node_t  *head = stack->head;

    *first = head->value.index;
    *second = head->next->value.index;
    *third = head->next->next->value.index;
    return (MAX(*first, MAX(*second, *third)));
}

static void
sort_two_b(helper_t *helper)
{
    stack_t *stack = helper->stack[1];

    MESSAGE("\nENTER sort_two_b\n");
    operator(PA, helper->stack[0], helper->stack[1]);
    operator(PA, helper->stack[0], helper->stack[1]);
    if (stack->head->value.index > stack->head->next->value.index)
        operator(SA, helper->stack[1], helper->stack[0]);
    MESSAGE("QUIT sort_two_b\n\n");
}

static void
sort_two_a(helper_t *helper)
{
    stack_t *stack = helper->stack[0];
    node_t  *head = stack->head;

    MESSAGE("\nENTER sort_two_a\n");
    if (head->value.index > head->next->value.index)
        operator(SA, helper->stack[0], helper->stack[1]);
    MESSAGE("QUIT sort_two_a\n\n");
}

static void
sort_three_b(helper_t *helper)
{
    size_t  first = 0, second = 0, third = 0, max = 0;
    stack_t *stack = helper->stack[1];

    max = init_three(helper->stack[0], &first, &second, &third);
    MESSAGE("\nENTER sort_three_b\n");
    if (max == third)
        operator(RRB, helper->stack[0], NULL);
    else if (max == second)
        operator(RB, helper->stack[0], NULL);
    operator(PA, helper->stack[0], helper->stack[1]);
    operator(PA, helper->stack[0], helper->stack[1]);
    operator(PA, helper->stack[0], helper->stack[1]);
    if (stack->head->value.index > stack->head->next->value.index)
        operator(SA, helper->stack[1], helper->stack[0]);
    MESSAGE("QUIT sort_three_b\n\n");
}

static void
sort_three_a(helper_t *helper)
{
    size_t  first = 0, second = 0, third = 0, max = 0;
    stack_t *stack = helper->stack[0];

    MESSAGE("\nENTER sort_three_a\n");
    max = init_three(stack, &first, &second, &third);
    if (max == first)
        operator(RA, helper->stack[0], NULL);
    else if (max == second)
        operator(RRA, helper->stack[0], NULL);
    if (stack->head->value.index > stack->head->next->value.index)
        operator(SA, helper->stack[0], helper->stack[1]);
    MESSAGE("QUIT sort_three_a\n\n");
}

static void
sort_three_front_a(helper_t *helper)
{
    size_t  first = 0, second = 0, third = 0, max = 0;
    stack_t *stack = helper->stack[0];

    MESSAGE("\nENTER sort_three_front_a\n");
    max = init_three(stack, &first, &second, &third);
    if (max == second) {
        operator(RA, helper->stack[0], NULL);
        operator(SA, helper->stack[0], helper->stack[1]);
        operator(RRA, helper->stack[0], NULL);
    } else if (max == first) {
        operator(SA, helper->stack[0], helper->stack[1]);
        operator(RA, helper->stack[0], NULL);
        operator(SA, helper->stack[0], helper->stack[1]);
        operator(RRA, helper->stack[0], NULL);
    }
    if (stack->head->value.index > stack->head->next->value.index)
        operator(SA, helper->stack[0], helper->stack[1]);
    MESSAGE("QUIT sort_three_front_a\n\n");
}

static void
sort_three_front_b(helper_t* helper)
{
    stack_t *stack1 = helper->stack[0], *stack2 = helper->stack[1];
    size_t  first = 0, second = 0, third = 0, max = 0;

    MESSAGE("\nENTER sort_three_front_b\n");
    max = init_three(stack1, &first, &second, &third);
    if (max == first)
        operator(PA, helper->stack[0], helper->stack[1]);
    else if (max == second) {
        operator(SB, helper->stack[0], helper->stack[1]);
        operator(PA, helper->stack[0], helper->stack[1]);
    } else {
        operator(RB, helper->stack[0], NULL);
        operator(SB, helper->stack[0], helper->stack[1]);
        operator(PA, helper->stack[0], helper->stack[1]);
        operator(RRB, helper->stack[0], NULL);
    }
    operator(PA, helper->stack[0], helper->stack[1]);
    operator(PA, helper->stack[0], helper->stack[1]);
    if (stack2->head->value.index > stack2->head->next->value.index)
        operator(SA, helper->stack[1], helper->stack[0]);
    MESSAGE("QUIT sort_three_front_b\n\n");
}

void
sort_partition(helper_t *helper)
{
    stack_t *stack = helper->stack[0];
    int len = helper->len[0];

    MESSAGE("\nENTER sort_partition\n");
    if (stack->id == 'a') {
        if (2 == len)
            sort_two_a(helper);
        else if (3 == stack->size)
            sort_three_a(helper);
        else
            sort_three_front_a(helper);
        set_node_sorted(helper->stack[0], len);
    } else {
        if (2 == len)
            sort_two_b(helper);
        else if (3 == stack->size)
            sort_three_b(helper);
        else
            sort_three_front_b(helper);
        set_node_sorted(helper->stack[1], len);
    }
    MESSAGE("QUIT sort_partition\n\n");
}