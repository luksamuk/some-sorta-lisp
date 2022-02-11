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

    dbg("atom table entry size: %lu bits\n", sizeof(atom_table_entry_t) * 8);
    dbg("cell size: %lu bits\n", sizeof(cell_t) * 8);
    dbg("atom table size: %lu bytes (approx. %lu MB)\n",
        sizeof(atom_table_t), sizeof(atom_table_t) / 1024 / 1024);
    dbg("list area size: %lu bytes (approx. %lu MB)\n",
        sizeof(list_area_t), sizeof(list_area_t) / 1024 / 1024);
    dbg("lisp vm size: %lu bytes (approx. %lu MB)\n",
        sizeof(lisp_vm_t), sizeof(lisp_vm_t) / 1024 / 1024);
    dbg("total atom table entries: %lu\n", ATOM_TABLE_SIZE);
    dbg("total list area cells: %lu\n", LIST_AREA_SIZE);
    dbg("\n");

    // Create atoms
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

    // Create pointers
    lisp_ptr_t p_nil    = make_pointer(TYPE_ATOM, a_nil);
    lisp_ptr_t p_t      = make_pointer(TYPE_ATOM, a_t);
    lisp_ptr_t p_quote  = make_pointer(TYPE_ATOM, a_quote);
    lisp_ptr_t p_lambda = make_pointer(TYPE_ATOM, a_lambda);
    lisp_ptr_t p_cond   = make_pointer(TYPE_ATOM, a_cond);
    lisp_ptr_t p_atom   = make_pointer(TYPE_ATOM, a_atom);
    lisp_ptr_t p_eq     = make_pointer(TYPE_ATOM, a_eq);
    lisp_ptr_t p_cdr    = make_pointer(TYPE_ATOM, a_cdr);
    lisp_ptr_t p_cons   = make_pointer(TYPE_ATOM, a_cons);

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

    // This is a little bolder.
    // v[A] = (LAMBDA () (CONS T (QUOTE B)))
    lisp_untptr_t a_A = make_atom(&vm->table, "a");
    lisp_untptr_t v_A;
    {
        lisp_ptr_t p_B = make_pointer(TYPE_ATOM, make_atom(&vm->table, "B"));
        v_A = make_pointer(
            TYPE_CONS,
            make_cons_cell(
                &vm->area,
                p_lambda,
                make_pointer(
                    TYPE_CONS,
                    make_cons_cell(
                        &vm->area,
                        p_nil,
                        make_pointer(
                            TYPE_CONS,
                            make_cons_cell(
                                &vm->area,
                                make_pointer(
                                    TYPE_CONS,
                                    make_cons_cell(
                                        &vm->area,
                                        p_cons,
                                        make_pointer(
                                            TYPE_CONS,
                                            make_cons_cell(
                                                &vm->area,
                                                p_t,
                                                make_pointer(
                                                    TYPE_CONS,
                                                    make_cons_cell(
                                                        &vm->area,
                                                        make_pointer(
                                                            TYPE_CONS,
                                                            make_cons_cell(
                                                                &vm->area,
                                                                p_quote,
                                                                make_pointer(
                                                                    TYPE_CONS,
                                                                    make_cons_cell(
                                                                        &vm->area,
                                                                        p_B, p_nil)))),
                                                        p_nil)))))),
                                p_nil))))));
    }
    lisp_ptr_t p_A = make_pointer(TYPE_CONS, v_A);
    bind_atom(&vm->table, a_A, p_A);
    
    printf("v[A] = ");
    print_object(&vm->table, &vm->area, p_A);
    putchar(10);
    
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

