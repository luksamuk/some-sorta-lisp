#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "def.h"
#include "util.h"
#include "types.h"
#include "list_area.h"

list_area_t
make_list_area()
{
    size_t byte_size = LIST_AREA_SIZE * sizeof(cell_t);

    dbg("starting list area with %lu cells (%lu bytes).\n",
        LIST_AREA_SIZE, byte_size);

    cell_t *area_ptr = malloc(byte_size);
    memset(area_ptr, 0, byte_size);
    
    list_area_t area;
    area.last = 0;
    area.area_ptr = area_ptr;

    return area;
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
    
lisp_ptr_t
make_pointer(lisp_ptr_tag_t tag, uint32_t value)
{
    uint32_t clean_value = value ^ (value & LISP_PTR_TAG_CLEAR_MASK);
    if(tag & TYPE_NUMBER != 0) {
        // Compensate signum
        uint32_t signum = value >> 31;
        clean_value |= (signum << 19);
    }
    uint32_t padded_tag = tag;
    padded_tag = padded_tag << 24;
    return padded_tag | clean_value;
}

void
clear_list_area(list_area_t *area)
{
    free(area->area_ptr);
}

lisp_ptr_tag_t
get_ptr_tag(lisp_ptr_t ptr)
{
    return ptr >> 24;
}

uint32_t
get_ptr_content(lisp_ptr_t ptr)
{
    return ptr ^ (ptr & LISP_PTR_TAG_CLEAR_MASK);
}

uint32_t
fix_numeric_content(lisp_ptr_t ptr_content)
{
    // FIXME: This is incorrect and won't work properly.
    uint32_t signum = ptr_content >> 23;
    ptr_content ^= 0x800000;
    ptr_content |= signum << 31;
    return ptr_content;
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
