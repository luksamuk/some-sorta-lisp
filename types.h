#ifndef LISP_TYPES_H
#define LISP_TYPES_H

#include <stdint.h>
#include "def.h"
#include "constants.h"

/// Type representing datatypes in general
typedef uint8_t datatype_t;

/// Generic untyped pointer
typedef uint32_t lisp_untptr_t;

/// Typed pointer to object in list area
typedef uint32_t lisp_ptr_t;

/// MSB of typed pointer representing the type tag
typedef uint8_t  lisp_ptr_tag_t;

/// Structure of a single entry on the atom table
typedef struct ATOM_TABLE_ENTRY_T {
    char          *name;
    lisp_untptr_t  __unused;
    lisp_ptr_t     value;
    lisp_untptr_t  plist;
    lisp_untptr_t  bindlist;
} atom_table_entry_t;

/// Single memory cell of list area
typedef struct CELL_T {
    lisp_ptr_t car;
    lisp_ptr_t cdr;
} cell_t;

/// Structure holding the state of an atom table
typedef struct ATOM_TABLE_T {
    lisp_untptr_t       last;
    lisp_untptr_t       __unused;
    atom_table_entry_t  table_ptr[ATOM_TABLE_SIZE];
} atom_table_t;

/// List area representation
typedef struct LIST_AREA_T {
    lisp_untptr_t  last;
    lisp_untptr_t  __unused;
    cell_t         area_ptr[LIST_AREA_SIZE];
} list_area_t;


const char *type_repr_str(datatype_t type);

#endif
