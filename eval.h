#ifndef LISP_EVAL_H
#define LISP_EVAL_H

#include "types.h"
#include "vm.h"

lisp_ptr_t vm_lookup(lisp_vm_t *vm, lisp_ptr_t atom);
lisp_ptr_t vm_proto_eval(lisp_vm_t *vm, lisp_ptr_t expr);

#endif
