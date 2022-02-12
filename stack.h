#ifndef DS_STACK_H
#define DS_STACK_H

#include <stdlib.h>

typedef struct STACK_NODE_T
{
    void                *info;
    struct STACK_NODE_T *prev;
} stack_node_t;

typedef struct
{
    size_t       element_size;
    size_t       num_elements;
    stack_node_t *top;
} stack_t;



stack_t    *make_stack(size_t);
int        stack_dispose(stack_t**);
int        stack_push(stack_t*, void*);
int        stack_pop(stack_t*);
const void *stack_peek(stack_t*);
void       stack_map(stack_t*, void (*func)(void *const));

#endif
