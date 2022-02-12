#include "eval.h"
#include "types.h"
#include "util.h"
#include "list_area.h"
#include "pointers.h"

lisp_ptr_t
vm_lookup(lisp_vm_t *vm, lisp_ptr_t atom)
{
    if(get_ptr_tag(atom) != TYPE_ATOM) {
        dbg("Error: pointer 0x%08X is not an atom\n", atom);
        return TP_NIL;
    }
    
    lisp_untptr_t addr = get_ptr_content(atom);
    return vm->table.table_ptr[addr].value;
}

lisp_ptr_t
vm_proto_eval(lisp_vm_t *vm, lisp_ptr_t expr)
{
    // Can only handle self-evaluating stuff and quote.
    // otherwise spit expression in verbatim

    switch(get_ptr_tag(expr)) {
    case TYPE_ATOM: return vm_lookup(vm, expr);
    case TYPE_NUMBER: return expr;
    case TYPE_CONS:
        if(get_car(&vm->area, get_ptr_content(expr)) == TP_QUOTE) {
            return get_car(
                &vm->area,
                get_ptr_content(
                    get_cdr(&vm->area, get_ptr_content(expr))));
        } else {
            return expr;
        }
        break;
    default: return expr;
    }
}
