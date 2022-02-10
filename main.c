#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "def.h"
#include "types.h"
#include "atom_table.h"
#include "atom.h"
#include "pointers.h"
#include "list_area.h"
#include "printing.h"
#include "vm.h"


void
main()
{
    lisp_vm_t *vm = make_lisp_vm();
    
    lisp_untptr_t a_nil     = make_atom(&vm->table, "nil");
    lisp_untptr_t a_t       = make_atom(&vm->table, "t");
    lisp_untptr_t a_quote   = make_atom(&vm->table, "quote");
    lisp_untptr_t a_atom    = make_atom(&vm->table, "atom");
    lisp_untptr_t a_eq      = make_atom(&vm->table, "eq");
    lisp_untptr_t a_car     = make_atom(&vm->table, "car");
    lisp_untptr_t a_cdr     = make_atom(&vm->table, "cdr");
    lisp_untptr_t a_cons    = make_atom(&vm->table, "cons");
    lisp_untptr_t a_cond    = make_atom(&vm->table, "cond");
    lisp_untptr_t a_lambda  = make_atom(&vm->table, "lambda");
    lisp_untptr_t a_label   = make_atom(&vm->table, "label");
    lisp_untptr_t a_special = make_atom(&vm->table, "special");

    // Create pointers to NIL and T
    lisp_ptr_t p_nil = make_pointer(TYPE_ATOM, a_nil);
    lisp_ptr_t p_t   = make_pointer(TYPE_ATOM, a_t);

    // Self-evaluating atoms NIL and T
    bind_atom(&vm->table, a_nil, p_nil);
    bind_atom(&vm->table, a_t, p_t);
    
    // v[MY-ATOM] = (5 . T)
    {
        lisp_untptr_t a_myatom = make_atom(&vm->table, "my-atom");
        lisp_untptr_t v_myatom = make_cons_cell(&vm->area,
                                                make_pointer(TYPE_NUMBER, 5),
                                                p_t);
        bind_atom(&vm->table, a_myatom, v_myatom);

        printf("v[(QUOTE MY-ATOM)] = ");
        print_object(&vm->table, &vm->area, make_pointer(TYPE_ATOM, a_myatom));
        putchar(10);
        
        printf("v[MY-ATOM] = ");
        print_object(&vm->table, &vm->area, make_pointer(TYPE_CONS, v_myatom));
        putchar(10);
    }

    // v[LIST-TEST] = (t . (t . (t . nil))) = (T T T)
    {
        lisp_untptr_t v_test;
        v_test = make_cons_cell(&vm->area, p_t, p_nil);
        
        v_test = make_cons_cell(&vm->area,
                                p_t,
                                make_pointer(TYPE_CONS, v_test));
        
        v_test = make_cons_cell(&vm->area,
                                p_t,
                                make_pointer(TYPE_CONS, v_test));
        
        lisp_untptr_t a_test = make_atom(&vm->table, "test");
        bind_atom(&vm->table, a_test, v_test);

        printf("v[LIST-TEST] = ");
        print_object(&vm->table, &vm->area, make_pointer(TYPE_CONS, v_test));
        putchar(10);
    }

    // v[NUMBER] = 93
    bind_atom(&vm->table,
              make_atom(&vm->table, "number"),
              make_pointer(TYPE_NUMBER, 5));

    printf("v[T] = ");
    print_object(&vm->table, &vm->area, p_t);
    putchar(10);

    printf("v[NIL] = ");
    print_object(&vm->table, &vm->area, p_nil);
    putchar(10);

    printf("v[32] = ");
    print_object(&vm->table, &vm->area, make_pointer(TYPE_NUMBER, 32));
    putchar(10);
    
    print_atom_table(&vm->table);
    print_list_area(&vm->area);

    clear_lisp_vm(vm);
}

