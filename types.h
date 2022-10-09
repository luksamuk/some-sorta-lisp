#ifndef LISP_TYPES_H
#define LISP_TYPES_H

#include <u.h>
#include <libc.h>
#include "def.h"
#include "constants.h"

/* ==================================================================== */
/* Primitive types for the lisp machine, wrapping other primitive types */
/* ==================================================================== */

/// Type representing types in general
typedef u8int datatype_t;

/// Generic untyped pointer
typedef u32int lisp_untptr_t;

/// Typed pointer to object in list area
typedef u32int lisp_ptr_t;

/// MSB of typed pointer representing the type tag
typedef u8int  lisp_ptr_tag_t;



/* ============================================ */
/* Primitive structures of the tables of the VM */
/* ============================================ */

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



/* ==================== */
/* Structures of the VM */
/* ==================== */

/// Structure representing the VM registers
typedef struct REGISTER_AREA_T {
    /// Expression to be evaluated
    lisp_ptr_t exp;
    /// Evaluation environment
    lisp_ptr_t env;
    /// Procedure to be applied
    lisp_ptr_t fun;
    /// List of evaluated arguments
    lisp_ptr_t argl;
    /// Place to go next
    lisp_ptr_t cont;
    /// Result of evaluation
    lisp_ptr_t val;
    /// Temporary register for expressions
    lisp_ptr_t unev;
} register_area_t;

/// Structure representing the VM stack
typedef struct STACK_AREA_T {
    lisp_untptr_t last;
    lisp_ptr_t    stack_ptr[LISP_STACK_SIZE];
} stack_area_t;

/// Structure holding the state of an atom table
typedef struct ATOM_TABLE_T {
    /// Untyped address ahead of last element
    lisp_untptr_t       last;
    lisp_untptr_t       __unused;
    /// Actual table area
    atom_table_entry_t  table_ptr[ATOM_TABLE_SIZE];
} atom_table_t;

/// List area representation
typedef struct LIST_AREA_T {
    /// Untyped address ahead of last element
    lisp_untptr_t  last;
    lisp_untptr_t  __unused;
    /// Actual list area
    cell_t         area_ptr[LIST_AREA_SIZE];
} list_area_t;



/* =============== */
/* Virtual Machine */
/* =============== */

// Representation of the Lisp VM
typedef struct LISP_VM_T {
    /// Area for VM registers
    register_area_t registers;
    /// Area for the system stack
    stack_area_t    stack;
    /// Area for registered atoms
    atom_table_t    table;
    /// Area for cons cells
    list_area_t     area;
} lisp_vm_t;

const char *type_repr_str(datatype_t type);

#endif
