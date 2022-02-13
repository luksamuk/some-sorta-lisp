#ifndef LISP_PRIMITIVES_H
#define LISP_PRIMITIVES_H

#include "types.h"

lisp_ptr_t lisp_spcl_quote(lisp_vm_t *vm, lisp_ptr_t env, lisp_ptr_t args);
// (ATOM X)
// (EQ X Y)
// (CAR X)
// (CDR X)
// (CONS X Y)
// (COND ...)
lisp_ptr_t lisp_spcl_setq(lisp_vm_t *vm, lisp_ptr_t env, lisp_ptr_t args);

#endif
