#include "push_swap.h"

static void
fill_array_with_stack(int *tab, node_t *head)
{
    node_t  *curr = head;
    int i = 0;

    do {
        tab[i] = curr->value.number;
        curr = curr->next;
        ++i;
    } while (curr != head);
}

static void
insertion_sort_on_array(int* tab, int len)
{
    int key, j;

    for (int i = 1; i < len; i += 1) {
        j = i - 1;
        key = tab[i];
        while (j >= 0 && tab[j] > key) {
            tab[j + 1] = tab[j];
            j = j - 1;
        }
        tab[j + 1] = key;
    }
}

static void
replace_stack_node_values(node_t *head, int* array)
{
    node_t  *curr = head;
    size_t  i;
    long    num;

    do {
        i = 0;
        num = curr->value.number;
        while (array[i] != num)
            ++i;
        curr->value.index = i;
        curr = curr->next;
    } while (curr != head);
}

void
replace_values_with_sorted_index(stack_t *a, error_t *error)
{
    node_t  *head = a->head;
    size_t  size = a->size;
    int *array = NULL;

    array = calloc(size, sizeof(int));
    if (NULL == array) *error = ALLOCATION;
    else {
        fill_array_with_stack(array, head);
        insertion_sort_on_array(array, size);
        replace_stack_node_values(head, array);
        free(array);
    }
}