#ifndef LISP_ATOM_H
#define LISP_ATOM_H

#include "types.h"

char          *make_atom_name(const char *given_name);
lisp_untptr_t  make_atom(atom_table_t *table, const char *atom_name);
lisp_untptr_t  bind_atom(atom_table_t *table, lisp_untptr_t atom, datatype_t type, lisp_untptr_t value);

const char    *get_atom_name(atom_table_t *table, lisp_untptr_t atom);

#endif
