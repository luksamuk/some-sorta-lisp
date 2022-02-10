#ifndef LISP_PRINTING_H
#define LISP_PRINTING_H

#include "types.h"

void print_object(atom_table_t *table, list_area_t *area, lisp_ptr_t ptr);
void print_list(atom_table_t *table, list_area_t *area, lisp_untptr_t untptr);

#endif
