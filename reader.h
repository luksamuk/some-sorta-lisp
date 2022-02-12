#ifndef LISP_READER_H
#define LISP_READER_H

#include "types.h"
#include "list.h"
#include "vm.h"

void       tokenize(const char *text, list_t *list);
lisp_ptr_t parse(lisp_vm_t *vm, list_t *tokens);
lisp_ptr_t read_expression(lisp_vm_t *vm, const char *text);

#endif
