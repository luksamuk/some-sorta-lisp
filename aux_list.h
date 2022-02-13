#ifndef DS_LIST_H
#define DS_LIST_H

#include <stdlib.h>

// This is an auxiliar implementation of a linked list I had laying around,
// and has nothing to do with the Lisp VM itself. I'm using it until I have
// a proper string implementation. By that time I can have a token list into
// the VM itself.

typedef struct LIST_NODE_T
{
    void               *info;
    struct LIST_NODE_T *next;
} list_node_t;

typedef struct
{
    size_t      element_size;
    size_t      num_elements;
    list_node_t *begin;
    list_node_t *end;
} list_t;



list_t     *make_list(size_t element_size);
int        list_append(list_t *list, void *item_ptr);
int        list_push(list_t *list, void *item_ptr);
int        list_insert_at(list_t *list, void *item_ptr, size_t pos);
const void *list_peek(list_t *list);
int        list_pop(list_t *list);
int        list_remove_at(list_t *list, size_t pos);
int        list_dispose(list_t **list_ptr);
void       list_map(list_t *list, void (*func)(void *const));
const void *list_item_at(list_t *list, size_t pos);

#endif
