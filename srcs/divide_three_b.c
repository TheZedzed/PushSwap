#include "push_swap.h"

static error_t
stop_divide_b(helper_t *helper, worker_t *worker, size_t *processed_nodes)
{
    size_t  tg_nodes = processed_nodes[2];
    size_t  sg_nodes = processed_nodes[1];
    size_t  fg_nodes = processed_nodes[0];
    stack_t	*stack = helper->stack[0];
    node_t	*head = stack->head;

    if (worker->len == 0
        || (tg_nodes == worker->parts_len[2]
            && ((sg_nodes == worker->parts_len[1])
                || (fg_nodes == worker->parts_len[0] && !sg_nodes)
                || (sg_nodes == worker->parts_len[1] && !fg_nodes))
        ))
    {
        MESSAGE("ENTER stop_divide_b\n");
        for (size_t i = 0; i < worker->len; i += 1)
        {
            head->partition = worker->subparts[!sg_nodes];
            head = head->next;
        }
        if (worker->parts_len[0] != stack->size)
        {
            if (fg_nodes > worker->parts_len[0] / 2)
                rotate_one(helper->stack[0], worker->parts_len[0] - fg_nodes);
            else
                reverse_rotate_one(helper->stack[0], fg_nodes);
        }
        MESSAGE("QUIT stop_divide_b");
        return (1);
    }
    return (0);
}

static int
need_rr(helper_t *helper, worker_t *worker, size_t *nodes)
{
    stack_t *b = helper->stack[0], *a = helper->stack[1];
    node_t  *head_a = a->head;

    operator(PA, helper->stack[0], helper->stack[1]);
    if (a->size > 1 && head_a->partition != worker->subparts[1])
    {
        if (worker->len > 1 && b->head->value.index < worker->pivots[0])
        {
            b->head->partition = worker->subparts[0];
            nodes[0] += 1;
            worker->len -= 1;
            return (2);
        }
        return (1);
    }
    return (0);
}

static void
proccess_first_part(helper_t *helper, worker_t *worker, size_t *nodes)
{
    node_t  *head = helper->stack[0]->head;

    nodes[0] += 1;
    head->partition = worker->subparts[0];
    operator(RB, helper->stack[0], NULL);
}

static void
proccess_second_part(helper_t *helper, worker_t *worker, size_t *nodes)
{
    node_t  *head = helper->stack[0]->head;
    int b_rr = 0;

    nodes[1] += 1;
    head->partition = worker->subparts[1];
    b_rr = need_rr(helper, worker, nodes);
    if (b_rr == 2)
        operator(RR, helper->stack[0], helper->stack[1]);
    else if (b_rr == 1)
        operator(RA, helper->stack[1], NULL);
}

static void
proccess_third_part(helper_t *helper, worker_t *worker, size_t *nodes)
{
    node_t  *head = helper->stack[0]->head;

    nodes[2] += 1;
    head->partition = worker->subparts[2];
    operator(PA, helper->stack[0], helper->stack[1]);
}

void
divide_three_in_b(helper_t *helper, worker_t *worker)
{
    size_t  processed_nodes[3] = {0, 0, 0};
    node_t  *head;
    size_t  value;

    MESSAGE("\nENTER divide_three_in_b\n");
    while (FAILURE == stop_divide_b(helper, worker, processed_nodes))
    {
        head = helper->stack[0]->head;
        value = head->value.index;
        if (value >= worker->pivots[1])
            proccess_third_part(helper, worker, processed_nodes);
        else if (value >= worker->pivots[0])
            proccess_second_part(helper, worker, processed_nodes);
        else
            proccess_first_part(helper, worker, processed_nodes);
        worker->len -= 1;
    }
    MESSAGE("\nQUIT divide_three_in_b\n");
}
