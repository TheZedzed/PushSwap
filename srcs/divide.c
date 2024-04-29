#include "push_swap.h"

static void
init_worker(helper_t *helper, worker_t *worker, int len, char id)
{
    worker->len = len;
    worker->subparts[0] = helper->partition[0] + 1;
    worker->subparts[1] = helper->partition[0] + 2;
    worker->subparts[2] = helper->partition[0] + 3;
    if (len < 6 || (id == 'a' && len == 6))
    {
        helper->partition[0] = worker->subparts[1];
        helper->partition[1] = worker->subparts[0];
        worker->pivots[0] = len / 2 + helper->min;
        helper->len[1] = worker->parts_len[0] = len / 2;
        helper->len[0] = worker->parts_len[1] = len - (len / 2);
    } else {
        helper->partition[0] = worker->subparts[2];
        helper->partition[1] = worker->subparts[1];
        worker->pivots[0] = len / 3 + helper->min;
        worker->pivots[1] = ((len / 3) * 2) + helper->min;
        worker->parts_len[0] = len / 3;
        worker->parts_len[2] = len - ((len / 3) * 2);
        worker->parts_len[1] = len - (len / 3) - (len - ((len / 3) * 2));
        helper->len[0] = worker->parts_len[2];
        helper->len[1] = worker->parts_len[1];
    }
}

void
divide_partition(helper_t *helper)
{
    static int  reverse_divide_order = 0;
    char    id = helper->stack[0]->id;
    int     len = helper->len[0];
    worker_t    worker = {};

    init_worker(helper, &worker, len, id);
    if ((id == 'a' && len == 6) || len < 6)
    {
        MESSAGE("\n\t\tChoose one pivot\n\n");
        if (id == 'a')
            divide_two_in_a(helper, &worker);
        else
            divide_two_in_b(helper, &worker);
    }
    else
    {
        MESSAGE("\n\t\tChoose two pivot\n\n");
        if (id == 'a')
            divide_three_in_a(helper, &worker, reverse_divide_order);
        else
        {
            reverse_divide_order = 1;
            divide_three_in_b(helper, &worker);
        }
    }
}