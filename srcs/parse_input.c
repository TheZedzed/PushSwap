#include "push_swap.h"

/* need hmap here */
static void
check_for_duplicate(stack_t *a, long number, error_t *error)
{
    node_t  *head = a->head;
    node_t  *last = head->prev;

    while (head != last) {
        if (number == head->value.number) {
            *error = DUPLICATE;
            break ;
        }
        head = head->next;
    }
}

static void
add_element(stack_t *a, long number, error_t *checker, error_t *error)
{
    node_t  *node = calloc(1, sizeof(node_t));

    if (NULL == node) *error = ALLOCATION;
    else {
        a->size += 1;
        node->value.number = number;
        if (NULL == a->head) {
            node->next = node->prev = node;
            a->head = node;
        } else {
            node_t  *last = a->head->prev;

            /* check if unsorted numbers */
            if (number < last->value.number && *checker == SORTED)
                *checker = SUCCESS;
            node->next = a->head;
            a->head->prev = node;
            node->prev = last;
            last->next = node;
        }
        check_for_duplicate(a, number, error);
    }
}

void
init_stack_with_input(int ac, char **av, stack_t *a, error_t *error)
{
    char    *token = NULL, *endptr = NULL, *delims = " \\t\\r\\v\\n\\f";
    error_t checker = SORTED;
    long    number;

    if (2 == ac) {
        token = strtok(av[1], delims);
        do {
            number = strtol(token, &endptr, 10);
            if (*endptr) *error = BAD_NUMBER;
            else add_element(a, number, &checker, error);
            token = strtok(NULL, delims);
        } while (NULL != token && SUCCESS == *error);
    } else {
        for (int index = 1; index < ac && SUCCESS == *error; index += 1) {
            number = strtol(av[index], &endptr, 10);
            if (*endptr) *error = BAD_NUMBER;
            else add_element(a, number, &checker, error);
        }
    }
    if (SORTED == checker)
        *error = checker;
}