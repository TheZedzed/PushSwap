#include "push_swap.h"

static error_t
stop_divide_a(helper_t *helper, worker_t *worker, size_t *processed_nodes)
{
    size_t  tg_nodes = processed_nodes[2];
    size_t  sg_nodes = processed_nodes[1];
    size_t  fg_nodes = processed_nodes[0];
    stack_t *stack = helper->stack[0];
    node_t  *head = stack->head;

    if (0 == worker->len
        || (sg_nodes == worker->parts_len[1]
            && fg_nodes == worker->parts_len[0]
        ))
    {
        MESSAGE("ENTER stop_divide_a\n");
        for (size_t i = 0; i < worker->len; i += 1)
        {
            head->partition = worker->subparts[2];
            head = head->next;
        }
        if (worker->parts_len[2] != helper->stack[0]->size)
            reverse_rotate_two(helper, tg_nodes, sg_nodes);
        MESSAGE("QUIT stop_divide_a");
        return (SUCCESS);
    }
    return (FAILURE);
}

static int
need_rr(helper_t *helper, worker_t *worker, size_t *nodes, int rev)
{
    stack_t *a = helper->stack[0], *b = helper->stack[1];
    node_t  *head_b = b->head;

    operator(PB, helper->stack[0], helper->stack[1]);
    if (b->size > 1
        && ((0 == rev && head_b->partition != worker->subparts[0])
        || (1 == rev && head_b->partition != worker->subparts[1])))
    {
        if (worker->len > 1 && a->head->value.index >= worker->pivots[1])
        {
            a->head->partition = worker->subparts[2];
            nodes[2] += 1;
            worker->len -= 1;
            return (2);
        }
        return (1);
    }
    return (0);
}

static void
proccess_first_part(helper_t *helper, worker_t *worker, size_t *nodes, int rev)
{
    node_t  *head = helper->stack[0]->head;
    int a_rr = 0;

    nodes[0] += 1;
    head->partition = worker->subparts[0];
    if (1 == rev)
        operator(PB, helper->stack[0], helper->stack[1]);
    else
    {
        a_rr = need_rr(helper, worker, nodes, rev);
        if (a_rr == 2)
            operator(RR, helper->stack[0], helper->stack[1]);
        else if (a_rr == 1)
            operator(RB, helper->stack[1], NULL);
    }
}

static void
proccess_second_part(helper_t *helper, worker_t *worker, size_t *nodes, int rev)
{
    node_t  *head = helper->stack[0]->head;
    int a_rr = 0;

    nodes[1] += 1;
    head->partition = worker->subparts[1];
    if (1 == rev)
    {
        a_rr = need_rr(helper, worker, nodes, rev);
        if (a_rr == 2)
            operator(RR, helper->stack[0], helper->stack[1]);
        else if (a_rr == 1)
            operator(RB, helper->stack[1], NULL);
    }
    else
        operator(PB, helper->stack[0], helper->stack[1]);
}

static void
proccess_third_part(helper_t *helper, worker_t *worker, size_t *nodes)
{
    node_t  *head = helper->stack[0]->head;

    nodes[2] += 1;
    head->partition = worker->subparts[2];
    operator(RA, helper->stack[0], NULL);
}

void
divide_three_in_a(helper_t *helper, worker_t *worker, int reverse)
{
    size_t  processed_nodes[3] = {0, 0, 0};
    node_t  *head;
    size_t  value;

    MESSAGE("\nENTER divide_three_in_a\n");
    while (FAILURE == stop_divide_a(helper, worker, processed_nodes))
    {
        head = helper->stack[0]->head;
        value = head->value.index;
        if (value >= worker->pivots[1])
            proccess_third_part(helper, worker, processed_nodes);
        else if (value >= worker->pivots[0])
            proccess_second_part(helper, worker, processed_nodes, reverse);
        else
            proccess_first_part(helper, worker, processed_nodes, reverse);
        worker->len -= 1;
    }
    MESSAGE("\nQUIT divide_three_in_a\n");
}
