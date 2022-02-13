#include "types.h"
#include "pointers.h"
#include "list_area.h"
#include "atom.h"
#include "eval.h"

lisp_ptr_t
lisp_spcl_quote(lisp_vm_t *vm, lisp_ptr_t env, lisp_ptr_t args)
{
    UNUSED(env);
    // TODO: Arity: 1
    return get_car(&vm->area, get_ptr_content(args));
}

lisp_ptr_t
lisp_spcl_setq(lisp_vm_t *vm, lisp_ptr_t env, lisp_ptr_t args)
{
    UNUSED(env);
    // TODO: Arity: 2
    lisp_ptr_t atom, value;
    // TODO: atom must be an atom
    atom = get_car(&vm->area, get_ptr_content(args));
    value = get_car(
        &vm->area,
        get_ptr_content(
            get_cdr(
                &vm->area,
                get_ptr_content(args))));
    value = vm_eval(vm, value);
    bind_atom(&vm->table, get_ptr_content(atom), value);
    return value;
}
