#include "vm_stack.h"
#include "atom_table.h"
#include "pointers.h"
#include <assert.h>

lisp_ptr_t
vm_stack_push(lisp_vm_t *vm, lisp_ptr_t ptr)
{
    if(vm->stack.last >= LISP_STACK_SIZE)
        return TP_NIL;
    vm->stack.stack_ptr[vm->stack.last++] = ptr;
    return TP_T;
}

lisp_ptr_t
vm_stack_peek(lisp_vm_t *vm)
{
    if(vm->stack.last == 0)
        return make_pointer(TYPE_ATOM, find_atom(&vm->table, "stack-underflow"));
    return vm->stack.stack_ptr[vm->stack.last - 1];
}

lisp_ptr_t
vm_stack_pop(lisp_vm_t *vm)
{
    if(vm->stack.last == 0)
        return make_pointer(TYPE_ATOM, find_atom(&vm->table, "stack-underflow"));
    vm->stack.last--;
    return vm->stack.stack_ptr[vm->stack.last];
}

void
vm_stack_unwind(lisp_vm_t *vm, lisp_ptr_t marker)
{
    while(vm->stack.stack_ptr[vm->stack.last] != marker) {
        if(vm->stack.last == 0) return;
        vm->stack.last--;
    }
    if(vm->stack.last > 0)
        vm->stack.last--;
}
