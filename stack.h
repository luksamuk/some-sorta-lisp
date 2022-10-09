#ifndef LISP_STACK_H
#define LISP_STACK_H

#include "types.h"

lisp_ptr_t  vm_stack_push(lisp_vm_t *vm, lisp_ptr_t ptr);
lisp_ptr_t  vm_stack_peek(lisp_vm_t *vm);
lisp_ptr_t  vm_stack_pop(lisp_vm_t *vm);
long        vm_stack_unwind(lisp_vm_t *vm, lisp_ptr_t marker);

#endif
