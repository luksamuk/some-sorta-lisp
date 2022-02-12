#include <stdlib.h>
#include <stdio.h>

#include "vm.h"
#include "util.h"
#include "atom.h"
#include "atom_table.h"
#include "pointers.h"
#include "list_area.h"

lisp_vm_t *
make_lisp_vm()
{
    lisp_vm_t *vm = malloc(sizeof(lisp_vm_t));
    init_atom_table(&vm->table);
    init_list_area(&vm->area);
    return vm;
}

void
init_lisp_vm(lisp_vm_t *vm)
{
    const char *PRIMITIVE_ATOMS[] = {
        "nil",
        "t",
        "quote",
        "atom"
        "eq",
        "car",
        "cdr",
        "cons",
        "cond",
        "lambda",
        "label",
        "special",
        NULL
    };
    
    const char **atom_ptr = PRIMITIVE_ATOMS;
    while((*atom_ptr) != NULL) {
        make_atom(&vm->table, *atom_ptr);
        atom_ptr++;
    }

    // Bind NIL and T as self-evaluating atoms
    bind_atom(&vm->table, 0, make_pointer(TYPE_ATOM, 0));
    bind_atom(&vm->table, 1, make_pointer(TYPE_ATOM, 1));

    debrief_vm(vm);
}

void
debrief_vm(lisp_vm_t *vm)
{
    size_t used_atom_table = vm->table.last * sizeof(atom_table_entry_t);
    size_t used_list_area  = vm->area.last * sizeof(cell_t);
    size_t total_size      = sizeof(atom_table_t) + sizeof(list_area_t);
    
    dbg("\nVM STATISTICS\n"
        "-------------\n"
        "ATOM TABLE: %10luB / %10luB\n"
        "LIST AREA:  %10luB / %10luB\n"
        "TOTAL SIZE: %10luB / %10luB\n\n",
        used_atom_table, sizeof(atom_table_t),
        used_list_area, sizeof(list_area_t),
        used_atom_table + used_list_area, total_size);
}

void
clear_lisp_vm(lisp_vm_t *vm)
{
    clear_atom_table(&vm->table);
    free(vm);
}
