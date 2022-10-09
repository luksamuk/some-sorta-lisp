#include "printing.h"
#include "atom.h"
#include "atom_table.h"
#include "pointers.h"
#include "list_area.h"

void
print_object(atom_table_t *table, list_area_t *area, lisp_ptr_t ptr)
{
    lisp_ptr_tag_t tag  = get_ptr_tag(ptr);
    u32int         cont = get_ptr_content(ptr);

    switch(tag) {
    case TYPE_UNDEFINED:
        print("<UNDEFINED>");
        break;
    case TYPE_NUMBER:
        print("%d", cont);
        break;
    case TYPE_ATOM:
        print("%s", get_atom_name(table, cont));
        break;
    case TYPE_FUNCTION:
        print("#<FUNCTION {0x%05X}>", cont);
        break;
    case TYPE_SPECIAL:
        print("#<SPECIAL {0x%05X}>", cont);
        break;
    case TYPE_BUILTIN_FUNCTION:
        print("#<BUILTIN-FUNCTION {0x%05X}>", cont);
        break;
    case TYPE_BUILTIN_SPECIAL:
        print("#<BUILTIN-SPECIAL {0x%05X}>", cont);
        break;
    case TYPE_CONS:
        print("(");
        print_list(table, area, cont);
        // Dotted pair notation
        /* print_object(table, area, area->area_ptr[cont].car); */
        /* print(" . "); */
        /* print_object(table, area, area->area_ptr[cont].cdr); */
        /* print(")"); */
        break;
        
    default: break;
    }
 }

void
print_list(atom_table_t *table, list_area_t *area, lisp_untptr_t untptr)
{
    lisp_ptr_t car = area->area_ptr[untptr].car;
    lisp_ptr_t cdr = area->area_ptr[untptr].cdr;
    
    print_object(table, area, car);

    lisp_ptr_tag_t cdr_tag  = get_ptr_tag(cdr);
    lisp_untptr_t  cdr_cont = get_ptr_content(cdr);
    
    if(cdr_tag == TYPE_CONS) {
        print(" ");
        print_list(table, area, get_ptr_content(cdr));
    } else if((cdr_tag == TYPE_ATOM) && (cdr_cont == 0))
        print(")");
    else {
        print(" . ");
        print_object(table, area, cdr);
        print(")");
    }
}
