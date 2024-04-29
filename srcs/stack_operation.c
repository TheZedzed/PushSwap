#include "push_swap.h"

static void
swap_node(node_t *x, node_t *y)
{
    size_t  data;
    int partition;

    partition = x->partition;
    data = x->value.index;
    x->partition = y->partition;
    x->value.index = y->value.index;
    y->partition = partition;
    y->value.index = data;
}

static void
remove_head(node_t **giver)
{
    node_t  *head = *giver, *last = NULL;

    if (*giver)
    {
        last = (*giver)->prev;
        if (head == last)
            *giver = NULL;
        else
        {
            *giver = head->next;
            last->next = *giver;
            (*giver)->prev = last;
        }
    }
}

static void
insert_head(node_t *new_head, node_t **receiver)
{
    node_t  *last = NULL;

    if (NULL == *receiver)
        new_head->next = new_head->prev = new_head;
    else
    {
        last = (*receiver)->prev;
        new_head->next = *receiver;
        new_head->prev = last;
        last->next = (*receiver)->prev = new_head;
    }
    *receiver = new_head;
}

static void
push_head(node_t **from, node_t **to)
{
    node_t  *from_head = *from;

    remove_head(from);
    insert_head(from_head, to);
}

static void
pa(stack_t *b, stack_t *a)
{
    MESSAGE("\tPA, node index '%zu'", b->head->value.index);
    MESSAGE(" partition '%d'\n", b->head->partition);
    push_head(&b->head, &a->head);
    b->size -= 1;
    a->size += 1;
    write(STDOUT_FILENO, "pa\n", 3);
}

static void
pb(stack_t *a, stack_t *b)
{
    MESSAGE("\tPB, node index '%zu'", a->head->value.index);
    MESSAGE(" partition '%d'\n", a->head->partition);
    push_head(&a->head, &b->head);
    a->size -= 1;
    b->size += 1;
    write(STDOUT_FILENO, "pb\n", 3);
}

static void
sa(stack_t *a, stack_t *b)
{
    swap_node(a->head, a->head->next);
    if (b->size > 1
        && b->head->value.index < b->head->next->value.index)
    {
        MESSAGE("\tSS,\n\t");
        MESSAGE("in stack 'a' node index '%zu'", a->head->next->value.index);
        MESSAGE(" partition '%d'", a->head->next->partition);
        MESSAGE(" with node index '%zu'\n\t", a->head->value.index);
        MESSAGE(" partition '%d'", a->head->partition);
        MESSAGE("in stack 'b' node index '%zu'", b->head->value.index);
        MESSAGE(" partition '%d'", b->head->partition);
        MESSAGE(" with node index '%zu'", b->head->next->value.index);
        MESSAGE(" partition '%d'\n", b->head->next->partition);
        swap_node(b->head, b->head->next);
        write(STDOUT_FILENO, "ss\n", 3);
    }
    else
    {
        MESSAGE("\tSA, node index '%zu'", a->head->next->value.index);
        MESSAGE(" partition '%d'", a->head->next->partition);
        MESSAGE(" with node index '%zu'", a->head->value.index);
        MESSAGE(" partition '%d'\n", a->head->partition);
        write(STDOUT_FILENO, "sa\n", 3);
    }
}

static void
sb(stack_t *b, stack_t *a)
{
    swap_node(b->head, b->head->next);
    if (a->size > 1
        && a->head->value.index > a->head->next->value.index)
    {
        MESSAGE("\tSS,\n\t");
        MESSAGE("in stack 'a' node index '%zu'", a->head->value.index);
        MESSAGE(" with node index '%zu'\n\t", a->head->next->value.index);
        MESSAGE("in stack 'b' node index '%zu'", b->head->next->value.index);
        MESSAGE(" with node index '%zu'\n", b->head->value.index);
        swap_node(a->head, a->head->next);
        write(STDOUT_FILENO, "ss\n", 3);
    }
    else
    {
        MESSAGE("\tSB, node index '%zu'", b->head->next->value.index);
        MESSAGE(" partition '%d'", b->head->next->partition);
        MESSAGE(" with node index '%zu'", b->head->value.index);
        MESSAGE(" partition '%d'\n", b->head->partition);
        write(STDOUT_FILENO, "sb\n", 3);
    }
}

static void
ra(stack_t *a, stack_t *b)
{
    (void)b;

    MESSAGE("\tRA, node index '%zu'", a->head->value.index);
    MESSAGE(" partition '%d'", a->head->partition);
    MESSAGE(" with node index '%zu'", a->head->next->value.index);
    MESSAGE(" partition '%d'\n", a->head->next->partition);
    a->head = a->head->next;
    write(STDOUT_FILENO, "ra\n", 3);
}

static void
rb(stack_t *b, stack_t *a)
{
    (void)a;

    MESSAGE("\tRB, node index '%zu'", b->head->value.index);
    MESSAGE(" partition '%d'", b->head->partition);
    MESSAGE(" with node index '%zu'", b->head->next->value.index);
    MESSAGE(" partition '%d'\n", b->head->next->partition);
    b->head = b->head->next;
    write(STDOUT_FILENO, "rb\n", 3);
}

static void
rr(stack_t *a, stack_t *b)
{
    MESSAGE("\tRR,\n\t");
    MESSAGE("in stack 'a' node index '%zu'", a->head->value.index);
    MESSAGE(" with node index '%zu'\n\t", a->head->next->value.index);
    MESSAGE("in stack 'b' node index '%zu'", b->head->value.index);
    MESSAGE(" with node index '%zu'\n", b->head->next->value.index);
    a->head = a->head->next;
    b->head = b->head->next;
    write(STDOUT_FILENO, "rr\n", 3);
}

static void
rra(stack_t *a, stack_t *b)
{
    (void)b;

    MESSAGE("\tRRA, node index '%zu'", a->head->value.index);
    MESSAGE(" partition '%d'", a->head->partition);
    MESSAGE(" with node index '%zu'", a->head->prev->value.index);
    MESSAGE(" partition '%d'\n", a->head->prev->partition);
    a->head = a->head->prev;
    write(STDOUT_FILENO, "rra\n", 4);
}

static void
rrb(stack_t *b, stack_t *a)
{
    (void)a;

    MESSAGE("\tRRB, node index '%zu'", b->head->value.index);
    MESSAGE(" partition '%d'", b->head->partition);
    MESSAGE(" with node index '%zu'", b->head->prev->value.index);
    MESSAGE(" partition '%d'\n", b->head->prev->partition);
    b->head = b->head->prev;
    write(STDOUT_FILENO, "rrb\n", 4);
}

static void
rrr(stack_t *a, stack_t *b)
{
    MESSAGE("\tRRR,\n\t");
    MESSAGE("in stack 'a' node index '%zu'", a->head->value.index);
    MESSAGE(" with node index '%zu'\n\t", a->head->prev->value.index);
    MESSAGE("in stack 'b' node index '%zu'", b->head->value.index);
    MESSAGE(" with node index '%zu'\n", b->head->prev->value.index);
    a->head = a->head->prev;
    b->head = b->head->prev;
    write(STDOUT_FILENO, "rrr\n", 4);
}

void
operator(operator_t operator, stack_t *a, stack_t *b)
{
    static t_exec   *f[10] = {pa, pb, sa, sb, ra, rb, rr, rra, rrb, rrr};

    if (operator < 10)
        f[operator](a, b);
}
