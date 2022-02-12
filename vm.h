#ifndef LISP_VM_H
#define LISP_VM_H

#include "def.h"
#include "types.h"

lisp_vm_t *make_lisp_vm();
void       init_lisp_vm(lisp_vm_t *vm);
void       debrief_vm(lisp_vm_t *vm);
void       clear_lisp_vm(lisp_vm_t *vm);

#endif
