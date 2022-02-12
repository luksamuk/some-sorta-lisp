#ifndef LISP_POINTERS_H
#define LISP_POINTERS_H

#include "types.h"

lisp_ptr_t     make_pointer(lisp_ptr_tag_t tag, uint32_t value);
lisp_ptr_tag_t get_ptr_tag(lisp_ptr_t ptr);
uint32_t       get_ptr_content(lisp_ptr_t ptr);
uint32_t       fix_numeric_content(uint32_t ptr_content);

#endif
