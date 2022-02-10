#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "def.h"
#include "util.h"
#include "types.h"
#include "pointers.h"
#include "list_area.h"

void
init_list_area(list_area_t *area_ptr)
{
    memset(area_ptr, 0, sizeof(list_area_t));
}

void
print_list_area(list_area_t *area)
{
    uint32_t current = 0;
    while(current < area->last) {
        if(current % 100 == 0) {
            printf("\n%5s | %14s | %14s\n",
                   "ADDR", "CAR", "CDR");
            int i = 0;
            while(i < 39) {
                putchar('-');
                i++;
            }
            putchar(10);
        }

        cell_t *cell = &area->area_ptr[current];
        printf("%05X | %5s | %06X | %0s | %06X\n",
               current,
               type_repr_str(get_ptr_tag(cell->car)),
               get_ptr_content(cell->car),
               type_repr_str(get_ptr_tag(cell->cdr)),
               get_ptr_content(cell->cdr));
        current++;
    }
}

lisp_untptr_t
make_cons_cell(list_area_t *area, lisp_ptr_t car, lisp_ptr_t cdr)
{
    uint32_t current = area->last;
    cell_t *cell_ptr = &area->area_ptr[current];
    area->last++;

    cell_ptr->car = car;
    cell_ptr->cdr = cdr;

    return current;
}
