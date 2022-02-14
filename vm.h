#ifndef LISP_VM_H
#define LISP_VM_H

#include "def.h"
#include "types.h"

lisp_vm_t *make_lisp_vm();
void       init_lisp_vm(lisp_vm_t *vm);
void       debrief_vm(lisp_vm_t *vm);
void       clear_lisp_vm(lisp_vm_t *vm);

// Util functions abstracting the VM
void       vm_push_state(lisp_vm_t *vm);
void       vm_pop_state(lisp_vm_t *vm);
lisp_ptr_t vm_get_atom(lisp_vm_t *vm, const char *name);
lisp_ptr_t vm_car(lisp_vm_t *vm, lisp_ptr_t ptr);
lisp_ptr_t vm_cdr(lisp_vm_t *vm, lisp_ptr_t ptr);
lisp_ptr_t vm_cadr(lisp_vm_t *vm, lisp_ptr_t ptr);
lisp_ptr_t vm_cddr(lisp_vm_t *vm, lisp_ptr_t ptr);
lisp_ptr_t vm_caddr(lisp_vm_t *vm, lisp_ptr_t ptr);
lisp_ptr_t vm_cons(lisp_vm_t *vm, lisp_ptr_t car, lisp_ptr_t cdr);

#endif
