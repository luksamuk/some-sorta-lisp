#include <stdlib.h>
#include <stdio.h>

#include "eval.h"
#include "types.h"
#include "util.h"
#include "list_area.h"
#include "pointers.h"
#include "reader.h"
#include "printing.h"

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
    case TYPE_ATOM:
        return vm_lookup(vm, expr);
    case TYPE_NUMBER:
        return expr;
    case TYPE_CONS:
        switch(get_car(&vm->area, get_ptr_content(expr))) {
        case TP_QUOTE:
            return get_car(
                &vm->area,
                get_ptr_content(
                    get_cdr(&vm->area, get_ptr_content(expr))));
        case TP_LAMBDA:
            // #<CONS (LAMBDA ARGS BODY)> => #<LAMBDA (ARGS BODY)>
            return make_pointer(
                TYPE_FUNCTION,
                get_ptr_content(get_cdr(&vm->area, get_ptr_content(expr))));
        case TP_SPECIAL:
            // #<CONS (SPECIAL ARGS BODY)> => #<SPECIAL (ARGS BODY)>
            return make_pointer(
                TYPE_FUNCTION,
                get_ptr_content(get_cdr(&vm->area, get_ptr_content(expr))));
        default:
            return expr;
        }
        break;
    default: return expr;
    }
}

void
vm_load_file(lisp_vm_t *vm, const char *filename)
{
    printf("Load file %s...\n", filename);
    char *file = slurp(filename);
    if(file) {
        lisp_ptr_t expr = read_expression(vm, file);
        free(file);
        
        while(expr != TP_NIL) {
            lisp_untptr_t content = get_ptr_content(expr);
            lisp_ptr_t result =
                vm_proto_eval(vm, get_car(&vm->area, content));
            print_object(&vm->table, &vm->area, result);
            putchar(10);
            expr = get_cdr(&vm->area, content);
        }
        putchar(10);
    }
}
