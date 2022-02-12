#include <stddef.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h>
#include "list.h"

#include "reader.h"
#include "atom.h"
#include "atom_table.h"
#include "list_area.h"
#include "pointers.h"
#include "vm.h"
#include "stack.h"

void
append_token(const char *token_buffer, list_t *list)
{
    list_append(list, make_atom_name(token_buffer));
}

void
tokenize(const char *text, list_t *list)
{
    char buffer[2048];
    size_t readpos = 0;
    size_t bufferpos = 0;
    
    while(text[readpos] != '\0') {
        char chr = text[readpos];
        if(isspace(chr) || (chr == '(') || (chr == ')')) {
            // Put token on buffer
            if(bufferpos > 0) {
                buffer[bufferpos] = '\0';
                append_token(buffer, list);
                bufferpos = 0;
            }
            if((chr == '(') || (chr == ')')) {
                append_token((chr == '(') ? "(" : ")", list);
            }
        } else buffer[bufferpos++] = chr;
        readpos++;
    }

    if(bufferpos > 0) {
        buffer[bufferpos] = '\0';
        append_token(buffer, list);
    }
}

lisp_ptr_t
parse(lisp_vm_t *vm, list_t *tokens)
{
    size_t i = 0;
    lisp_ptr_t p_nil = make_pointer(TYPE_ATOM, find_atom(&vm->table, "nil"));
    lisp_ptr_t curr = p_nil;
    lisp_ptr_t root = p_nil;
    
    stack_t *stack = make_stack(sizeof(lisp_ptr_t));
    
    while(i < tokens->num_elements) {
        char *token = (char*)list_item_at(tokens, i);
        if(!strcmp(token, "(")) {
            // Push registers onto stack
            stack_push(stack, &root);
            stack_push(stack, &curr);
            curr = p_nil;
            root = p_nil;
        } else if(!strcmp(token, ")")) {
            // TODO: This will fail if stack underflows!
            lisp_ptr_t new, tmp;
            // Prepare list of accumulated elements to join parent
            new = make_pointer(TYPE_CONS, make_cons_cell(&vm->area, root, p_nil));
            // Pop old curr into tmp
            tmp = *((lisp_ptr_t*)stack_peek(stack));
            stack_pop(stack);
            // If something was popped, set old curr's tail to
            // the list prepared earlier
            if(tmp != p_nil) {
                cell_set_cdr(&vm->area, get_ptr_content(tmp), new);
            }
            // Continue accumulating elements with the prepared list
            // as current tail
            curr = new;
            // Restore old root value
            root = *((lisp_ptr_t*)stack_peek(stack));
            stack_pop(stack);
            // If there is nothing on root, take curr as first element
            // of a newly created list
            if(root == p_nil) {
                root = curr;
            }
        } else if(isdigit(token[0])
                  || ((token[0] == '-') && isdigit(token[1]))) {
            lisp_ptr_t new, tmp;
            // Generate number
            new = make_pointer(TYPE_NUMBER, atoi(token));
            // Put number onto a new cell head
            tmp = make_pointer(TYPE_CONS, make_cons_cell(&vm->area, new, p_nil));
            if(curr != p_nil) {
                // If curr is a cell, set its tail to the new cell
                cell_set_cdr(&vm->area, get_ptr_content(curr), tmp);
            } else {
                // Otherwise, since curr will be set for the first time,
                // update the root as well so we don't lose its reference
                root = tmp;
            }
            // Set curr to newly created cell
            curr = tmp;
        } else {
            lisp_ptr_t new, cell;
            new = make_pointer(TYPE_ATOM, find_atom(&vm->table, token));
            // Append new atom onto Lisp list.
            // Works much like number parsing. See it for details
            cell = make_pointer(TYPE_CONS, make_cons_cell(&vm->area, new, p_nil));
            if(curr != p_nil)
                cell_set_cdr(&vm->area, get_ptr_content(curr), cell);
            else
                root = cell;
            curr = cell;
        }
        i++;
    }
    // TODO: If stack is not empty, we got unbalanced parentheses
    stack_dispose(&stack);
    return get_car(&vm->area, get_ptr_content(curr));
}

lisp_ptr_t
read_expression(lisp_vm_t *vm, const char *text)
{
    list_t *tokens = make_list(sizeof(char*));

    tokenize(text, tokens);
    lisp_ptr_t expr = parse(vm, tokens);
    
    list_dispose(&tokens);
    return expr;
}

char *
slurp(const char *filename)
{
    char *buffer = NULL;
    long length;
    FILE *file = fopen(filename, "r");

    if(!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = malloc(length);
    
    if(buffer)
        fread(buffer, 1, length, file);

    fclose(file);
    return buffer;
}
