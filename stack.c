#include "stack.h"
#include "atom_table.h"
#include "pointers.h"

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

size_t
vm_stack_unwind(lisp_vm_t *vm, lisp_ptr_t marker)
{
    size_t popped = 0;
    while(vm->stack.stack_ptr[vm->stack.last] != marker) {
        if(vm->stack.last == 0)
            goto end;
        vm->stack.last--;
        popped++;
    }
    if(vm->stack.last > 0)
        vm->stack.last--;
end:
    return popped;
}
