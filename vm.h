#ifndef LISP_VM_H
#define LISP_VM_H

#include "def.h"
#include "types.h"

// Representation of the Lisp VM
typedef struct LISP_VM_T {
    atom_table_t table;
    list_area_t  area;
} lisp_vm_t;

lisp_vm_t *make_lisp_vm();
void       clear_lisp_vm(lisp_vm_t *vm);

#endif
