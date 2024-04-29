#include "push_swap.h"

static error_t
stop_divide_b(helper_t *helper, worker_t *worker, size_t *processed_nodes)
{
    size_t  sg_nodes = processed_nodes[1];
    size_t  fg_nodes = processed_nodes[0];
    stack_t *stack = helper->stack[0];
    node_t  *head = stack->head;

    if (worker->len == 0 || (sg_nodes == worker->parts_len[1] && !fg_nodes))
    {
        MESSAGE("ENTER stop_divide_b\n");
        for (size_t i = 0; i < worker->len; i += 1)
        {
            head->partition = worker->subparts[0];
            head = head->next;
        }
        MESSAGE("QUIT stop_divide_b");
        return (SUCCESS);
    }
    return (FAILURE);
}

void
divide_two_in_b(helper_t *helper, worker_t *worker)
{
    size_t  processed_nodes[2] = {0, 0};
    node_t  *head;
    size_t  value;

    MESSAGE("\nENTER divide_two_in_b\n");
    while (FAILURE == stop_divide_b(helper, worker, processed_nodes))
    {
        head = helper->stack[0]->head;
        value = head->value.index;
        if (value >= worker->pivots[0])
        {
            processed_nodes[1] += 1;
            head->partition = worker->subparts[1];
            operator(PA, helper->stack[0], helper->stack[1]);
        }
        else
        {
            processed_nodes[0] += 1;
            head->partition = worker->subparts[0];
            operator(PA, helper->stack[0], helper->stack[1]);
            operator(RA, helper->stack[1], NULL);
        }
        worker->len -= 1;
    }
    MESSAGE("\nQUIT divide_two_in_b\n");
}
