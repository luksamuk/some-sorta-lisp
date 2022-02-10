#ifndef LISP_ATOM_TABLE_H
#define LISP_ATOM_TABLE_H

#include "types.h"

void init_atom_table(atom_table_t *table_ptr);
void clear_atom_table(atom_table_t *table);
void print_atom_table(atom_table_t *table);

#endif
