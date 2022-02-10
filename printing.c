#include <stdio.h>

#include "printing.h"
#include "atom.h"
#include "atom_table.h"
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
    case TYPE_ATOM:
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
