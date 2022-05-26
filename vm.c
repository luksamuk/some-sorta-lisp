#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vm.h"
#include "util.h"
#include "atom.h"
#include "atom_table.h"
#include "pointers.h"
#include "stack.h"
#include "list_area.h"

lisp_vm_t *
make_lisp_vm()
{
    lisp_vm_t *vm = malloc(sizeof(lisp_vm_t));
    memset(&vm->registers, 0, sizeof(register_area_t));
    init_atom_table(&vm->table);
    init_list_area(&vm->area);
    return vm;
}

void
init_lisp_vm(lisp_vm_t *vm)
{
    const char *PRIMITIVE_ATOMS[] = {
        // DO NOT change the order!
        "nil",
        "t",
        "quote",
        "atom",
        "eq",
        "car",
        "cdr",
        "cons",
        "cond",
        "lambda",
        "special",
        "setq",
        "undefined",
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

    // Bind primitive procedures
    lisp_ptr_t plus = vm_get_atom(vm, "+");
    bind_atom(&vm->table, get_ptr_content(plus),
              make_pointer(TYPE_BUILTIN_FUNCTION,
                           get_ptr_content(plus)));
    lisp_ptr_t ttypeof = vm_get_atom(vm, "typeof");
    bind_atom(&vm->table, get_ptr_content(ttypeof),
	      make_pointer(TYPE_BUILTIN_FUNCTION,
	                   get_ptr_content(ttypeof)));
}

void
debrief_vm(lisp_vm_t *vm)
{
    size_t used_atom_table = vm->table.last * sizeof(atom_table_entry_t);
    size_t used_list_area  = vm->area.last * sizeof(cell_t);
    size_t total_size      = sizeof(atom_table_t) + sizeof(list_area_t);
    
    printf("\nVM STATISTICS\n"
           "-------------\n"
           "ATOM TABLE: %10luB / %10luB (%u atoms)\n"
           "LIST AREA:  %10luB / %10luB (%u cells)\n"
           "TOTAL SIZE: %10luB / %10luB\n\n",
           used_atom_table, sizeof(atom_table_t), vm->table.last,
           used_list_area, sizeof(list_area_t), vm->area.last,
           used_atom_table + used_list_area, total_size);
}

void
clear_lisp_vm(lisp_vm_t *vm)
{
    clear_atom_table(&vm->table);
    free(vm);
}

void
vm_push_state(lisp_vm_t *vm)
{
    // For evaluation effects, all registers are pushed
    // but one, since VAL holds the result of an evaluation.
    vm_stack_push(vm, vm->registers.exp);
    vm_stack_push(vm, vm->registers.env);
    vm_stack_push(vm, vm->registers.fun);
    vm_stack_push(vm, vm->registers.argl);
    vm_stack_push(vm, vm->registers.cont);
    vm_stack_push(vm, vm->registers.unev);

    // Clean up registers
    vm->registers.exp =
        vm->registers.env =
        vm->registers.fun =
        vm->registers.argl =
        vm->registers.cont =
        vm->registers.unev = TP_NIL;
}

void
vm_pop_state(lisp_vm_t *vm)
{
    // For evaluation effects, restore the state of all
    // registers but VAL. We should not destroy it since
    // it will hold the result of the evaluation.
    vm->registers.unev = vm_stack_pop(vm);
    vm->registers.cont = vm_stack_pop(vm);
    vm->registers.argl = vm_stack_pop(vm);
    vm->registers.fun  = vm_stack_pop(vm);
    vm->registers.env  = vm_stack_pop(vm);
    vm->registers.exp  = vm_stack_pop(vm);
}

lisp_ptr_t
vm_get_atom(lisp_vm_t *vm, const char *name)
{
    return make_pointer(TYPE_ATOM, find_atom(&vm->table, name));
}

lisp_ptr_t
vm_car(lisp_vm_t *vm, lisp_ptr_t ptr)
{
    return get_car(&vm->area, get_ptr_content(ptr));
}

lisp_ptr_t
vm_cdr(lisp_vm_t *vm, lisp_ptr_t ptr)
{
    return get_cdr(&vm->area, get_ptr_content(ptr));
}

lisp_ptr_t
vm_cadr(lisp_vm_t *vm, lisp_ptr_t ptr)
{
    return vm_car(vm, vm_cdr(vm, ptr));
}

lisp_ptr_t
vm_cddr(lisp_vm_t *vm, lisp_ptr_t ptr)
{
    return vm_cdr(vm, vm_cdr(vm, ptr));
}

lisp_ptr_t
vm_caddr(lisp_vm_t *vm, lisp_ptr_t ptr)
{
    return vm_car(vm, vm_cddr(vm, ptr));
}

lisp_ptr_t
vm_cons(lisp_vm_t *vm, lisp_ptr_t car, lisp_ptr_t cdr)
{
    return make_pointer(TYPE_CONS, make_cons_cell(&vm->area, car, cdr));
}
