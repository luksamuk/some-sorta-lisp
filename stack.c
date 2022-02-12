#include "stack.h"
#include <string.h>

stack_t*
make_stack(size_t element_size)
{
    stack_t *new_stack = malloc(sizeof(stack_t));
    new_stack->element_size = element_size;
    new_stack->num_elements = 0;
    new_stack->top          = NULL;
    return new_stack;
}

int
stack_dispose(stack_t **stack_ptr)
{
    if(!stack_ptr || !(*stack_ptr))
        return 1;

    stack_t* stack = *stack_ptr;
    while(stack->num_elements > 0)
        stack_pop(stack);
    
    free(stack);
    *stack_ptr = NULL;
    return 0;
}

int
stack_push(stack_t *stack, void *info)
{
    if(!stack || !info)
        return 1;

    stack_node_t *node = malloc(sizeof(stack_node_t));
    if(!node) return 1;

    node->info = malloc(stack->element_size);
    if(!node->info) {
        free(node);
        return 1;
    }

    memcpy(node->info, info, stack->element_size);
    stack->num_elements++;

    if(!stack->top) {
        node->prev = NULL;
        stack->top = node;
        return 0;
    }

    node->prev = stack->top;
    stack->top = node;
    return 0;
}

int
stack_pop(stack_t *stack)
{
    if(!stack || !stack->top)
        return 1;

    stack_node_t *prev = stack->top->prev;
    free(stack->top);
    stack->top = prev;
    stack->num_elements--;
    return 0;
    
}

const void*
stack_peek(stack_t *stack)
{
    if(!stack || !stack->top)
        return NULL;
    return stack->top->info;
        
}

void
stack_map(stack_t *stack, void (*func)(void *const))
{
    if(!stack) return;
    stack_node_t *itr;
    for(itr = stack->top; itr != NULL; itr = itr->prev) {
        func(itr->info);
    }
}
