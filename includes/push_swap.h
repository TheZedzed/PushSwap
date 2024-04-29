#ifndef PUSH_SWAP_H
#define PUSH_SWAP_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#ifdef DEBUG
#define MESSAGE(...) fprintf(stderr, __VA_ARGS__);
#else
#define MESSAGE(...)
#endif

/* helper in error management */
typedef enum    error {
    SORTED = -1,
    FAILURE = 0,
    SUCCESS,
    ALLOCATION,
    NUMBER_ARGUMENTS,
    BAD_NUMBER,
    DUPLICATE
} error_t;

/* stack operators */
typedef enum    operator {
    PA = 0,
    PB,
    SA,
    SB,
    RA,
    RB,
    RR,
    RRA,
    RRB,
    RRR
} operator_t;

/* stores the relation between the unsorted number <=> index when sorted */
typedef union   node_data {
    long    number;
    size_t  index;
} node_data_t;

/* represent a node in a stack */
typedef struct  node_s {
    int partition;
    node_data_t     value;
    struct node_s   *prev;
    struct node_s   *next;
} node_t;

/* represent a stack */
typedef struct  stack_s {
    char  id;
    size_t  size;
    node_t  *head;
} stack_t;

/* structure for divide helper partitions into subparts */
typedef struct  worker
{
    size_t len;
    int subparts[3];
    size_t pivots[2];
    size_t parts_len[3];
} worker_t;

/* structure with the stacks biggest partitions and their length */
typedef struct  helper
{
    size_t min;
    size_t len[2];
    int partition[2];
    stack_t *stack[2];
} helper_t;

/* operator function pointer */
typedef void    (t_exec) (stack_t*, stack_t*);

void    operator(operator_t, stack_t*, stack_t*);

void    init_stack_with_input(int ac, char **av, stack_t *a, error_t *error);
void    replace_values_with_sorted_index(stack_t *a, error_t *error);

error_t choose_partition(helper_t *helper, stack_t *a, stack_t *b);
error_t partition_is_sorted(helper_t *helper);
void    put_sorted_partiton_in_a(helper_t *helper);

int     quit(stack_t**, stack_t**, int);
void    set_node_sorted(stack_t *stack, int len);
void    reverse_rotate_two(helper_t *helper, int tg_nodes, int sg_nodes);
void    reverse_rotate_one(stack_t *stack, int len);
void    rotate_one(stack_t *stack, int len);

void    sort_partition(helper_t *helper);
void    divide_partition(helper_t *helper);

void    divide_two_in_b(helper_t *helper, worker_t *worker);
void    divide_two_in_a(helper_t *helper, worker_t *worker);
void    divide_three_in_b(helper_t *helper, worker_t *worker);
void    divide_three_in_a(helper_t *helper, worker_t *worker, int reverse);

#endif
