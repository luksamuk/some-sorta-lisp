#include "list.h"
#include <stdlib.h>
#include <string.h>

list_t*
make_list(size_t element_size)
{
    if(element_size == 0)
        return NULL;
    
    list_t *list;
    list = malloc(sizeof(list_t));

    if(!list) return NULL;

    list->element_size = element_size;
    list->num_elements = 0;
    list->begin = list->end = NULL;

    return list;
}

int
list_append(list_t *list, void *item_ptr)
{
    if(!item_ptr || !list) return 0;

    list_node_t *itr;
    if(!list->begin) {
        itr = malloc(sizeof(list_node_t));
        if(!itr) return 0;
        
        itr->info = malloc(list->element_size);
        if(!itr->info) {
            free(itr);
            return 0;
        }
        
        list->begin = list->end = itr;
    } else {
        itr = list->end;
        
        itr->next = malloc(sizeof(list_node_t));
        if(!itr->next) return 0;
        
        itr->next->info = malloc(list->element_size);
        if(!itr->next->info) {
            free(itr->next);
            itr->next = NULL;
            return 0;
        }

        itr = itr->next;
        list->end = itr;
    }

    itr->next = NULL;
    memcpy((char*)itr->info, (char*)item_ptr, list->element_size);
    list->num_elements++;

    return 1;
}

int
list_push(list_t *list, void *item_ptr)
{
    if(!item_ptr || !list) return 0;
    
    list_node_t *itr;
    
    itr = malloc(sizeof(list_node_t));
    if(!itr) return 0;

    itr->info = malloc(list->element_size);
    if(!itr->info) {
        free(itr);
        return 0;
    }

    memcpy((char*)itr->info, (char*)item_ptr, list->element_size);
    itr->next = list->begin;
    list->begin = itr;

    list->num_elements++;
    
    return 1;
}

int
list_insert_at(list_t *list, void *item_ptr, size_t pos) {
    if(pos == 0) {
        return list_push(list, item_ptr);
    }

    if(!list || !item_ptr || (list->num_elements <= pos))
        return 0;

    if(pos == list->num_elements) {
        return list_append(list, item_ptr);
    }

    list_node_t *itr, *new_node;
    size_t i;
    for(i = 0, itr = list->begin;
        i < pos - 1;
        i++, itr = itr->next) {}

    new_node = malloc(sizeof(list_node_t));
    if(!new_node) return 0;

    new_node->info = malloc(list->element_size);
    if(!new_node->info) {
        free(new_node);
        return 0;
    }

    memcpy((char*)new_node->info, (char*)item_ptr, list->element_size);
    new_node->next = itr->next;
    itr->next = new_node;
    list->num_elements++;

    return 1;
}

const void*
list_peek(list_t *list)
{
    if(!list || (list->num_elements == 0))
        return NULL;
    return list->begin->info;
}

int
list_pop(list_t *list)
{
    if(!list || !list->begin)
        return 0;

    if(list->begin == list->end) {
        list->end = NULL;
    }

    list_node_t *itr = list->begin->next;
    free(list->begin->info);
    free(list->begin);
    list->begin = itr;
    list->num_elements--;
    
    return 1;
}

int
list_remove_at(list_t *list, size_t pos)
{
    if(pos == 0) {
        return list_pop(list);
    }
    
    if(!list || (list->num_elements <= pos))
       return 0;

    list_node_t *itr, *new_next;
    size_t i;

    for(i = 0, itr = list->begin;
        i < pos - 1;
        i++, itr = itr->next) {}
    
    new_next = itr->next->next;
    
    if(list->end == itr->next)
        list->end = itr;
    
    free(itr->next->info);
    free(itr->next);
    itr->next = new_next;
    list->num_elements--;

    return 1;
}

int
list_dispose(list_t **list_ptr)
{
    if(!list_ptr) return 0;

    list_t *the_list = *list_ptr;
    if(!the_list) return 0;
    
    while(the_list->num_elements > 0)
        list_pop(the_list);

    free(the_list);
    *list_ptr = NULL;

    return 1;
}

void
list_map(list_t *list, void (*func)(void *const))
{
    if(!list || !func) return;
    
    list_node_t *itr;
    for(itr = list->begin; itr != NULL; itr = itr->next) {
        func(itr->info);
    }
}

const void*
list_item_at(list_t *list, size_t pos)
{
    if(pos == 0) {
        return list_peek(list);
    }
    
    if(!list || (list->num_elements <= pos))
        return NULL;

    if(pos == (list->num_elements - 1)) {
        return list->end->info;
    }
    
    list_node_t *itr;
    size_t i;
    for(i = 0, itr = list->begin;
        i < pos;
        i++, itr = itr->next) {}

    return itr->info;
}
