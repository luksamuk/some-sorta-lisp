#include <stdio.h>

#include "util.h"
#include "def.h"
#include "types.h"
#include "atom_table.h"
#include "atom.h"
#include "list_area.h"

void
print_object(atom_table_t *table, list_area_t *area, lisp_ptr_t ptr)
{
    lisp_ptr_tag_t tag  = get_ptr_tag(ptr);
    uint32_t       cont = get_ptr_content(ptr);

    switch(tag) {
    case TYPE_NUMBER:
        printf("%d", cont);
        break;
    case TYPE_VARIABLE:
        printf("%s", get_atom_name(table, cont));
        break;
    case TYPE_CONS:
        putchar('(');
        print_object(table, area, area->area_ptr[cont].car);
        printf(" . ");
        print_object(table, area, area->area_ptr[cont].cdr);
        putchar(')');
        break;
    default: break;
    }
 }

void
main()
{
    atom_table_t table = make_atom_table();
    list_area_t  area  = make_list_area();
    putchar(10);
    
    lisp_untptr_t a_nil     = make_atom(&table, "nil");
    lisp_untptr_t a_t       = make_atom(&table, "t");
    lisp_untptr_t a_quote   = make_atom(&table, "quote");
    lisp_untptr_t a_atom    = make_atom(&table, "atom");
    lisp_untptr_t a_eq      = make_atom(&table, "eq");
    lisp_untptr_t a_car     = make_atom(&table, "car");
    lisp_untptr_t a_cdr     = make_atom(&table, "cdr");
    lisp_untptr_t a_cons    = make_atom(&table, "cons");
    lisp_untptr_t a_cond    = make_atom(&table, "cond");
    lisp_untptr_t a_lambda  = make_atom(&table, "lambda");
    lisp_untptr_t a_label   = make_atom(&table, "label");
    lisp_untptr_t a_special = make_atom(&table, "special");

    // Self-evaluating atoms NIL and T
    bind_atom(&table, a_nil, TYPE_VARIABLE, a_nil);
    bind_atom(&table, a_t, TYPE_VARIABLE, a_t);

    // Create pointers to NIL and T
    lisp_ptr_t p_nil = make_pointer(TYPE_VARIABLE, a_nil);
    lisp_ptr_t p_t   = make_pointer(TYPE_VARIABLE, a_t);
    
    // v[MY-ATOM] = (5 . T)
    {
        lisp_untptr_t a_myatom = make_atom(&table, "my-atom");
        lisp_untptr_t v_myatom = make_cons_cell(&area,
                                                make_pointer(TYPE_NUMBER, 5),
                                                p_t);
        bind_atom(&table, a_myatom, TYPE_CONS, v_myatom);

        printf("v[(QUOTE MY-ATOM)] = ");
        print_object(&table, &area, make_pointer(TYPE_VARIABLE, a_myatom));
        putchar(10);
        
        printf("v[MY-ATOM] = ");
        print_object(&table, &area, make_pointer(TYPE_CONS, v_myatom));
        putchar(10);
    }

    // v[LIST-TEST] = (t . (t . (t . nil))) = (T T T)
    {
        lisp_untptr_t v_test;
        v_test = make_cons_cell(&area, p_t, p_nil);
        
        v_test = make_cons_cell(&area,
                                p_t,
                                make_pointer(TYPE_CONS, v_test));
        
        v_test = make_cons_cell(&area,
                                p_t,
                                make_pointer(TYPE_CONS, v_test));
        
        lisp_untptr_t a_test = make_atom(&table, "test");
        bind_atom(&table, a_test, TYPE_CONS, v_test);

        printf("v[LIST-TEST] = ");
        print_object(&table, &area, make_pointer(TYPE_CONS, v_test));
        putchar(10);
    }

    printf("v[T] = ");
    print_object(&table, &area, p_t);
    putchar(10);

    printf("v[NIL] = ");
    print_object(&table, &area, p_nil);
    putchar(10);

    printf("v[32] = ");
    print_object(&table, &area, make_pointer(TYPE_NUMBER, 32));
    putchar(10);
    
    print_atom_table(&table);
    print_list_area(&area);

    clear_list_area(&area);
    clear_atom_table(&table);
}

