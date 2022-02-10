#include <stdlib.h>
#include <stdio.h>

#include "vm.h"
#include "atom_table.h"
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
clear_lisp_vm(lisp_vm_t *vm)
{
    clear_atom_table(&vm->table);
    free(vm);
}
