#ifndef LISP_EVAL_H
#define LISP_EVAL_H

#include "types.h"
#include "vm.h"

lisp_ptr_t vm_lookup(lisp_vm_t *vm, lisp_ptr_t atom);
lisp_ptr_t vm_eval(lisp_vm_t *vm, lisp_ptr_t expr);
lisp_ptr_t vm_explicit_eval(lisp_vm_t *vm);

void       vm_load_file(lisp_vm_t *vm, const char *filename);

#endif
