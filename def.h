#ifndef LISP_DEF_H
#define LISP_DEF_H

/// Size of atom table entry: 24B (192 bit)
/// Size of list area cons cell: 8B (64 bit)
/// Size of immediate number store: 24 bit

#define ATOM_TABLE_SIZE            3200000
#define LIST_AREA_SIZE            10000000
#define LISP_PTR_TAG_CLEAR_MASK 0xFF000000
#define LISP_STACK_SIZE            8388608 // 8MB stack

#define UNUSED(x) (void)(x)

#endif
