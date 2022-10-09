#ifndef LISP_POINTERS_H
#define LISP_POINTERS_H

#include "types.h"

lisp_ptr_t     make_pointer(lisp_ptr_tag_t tag, lisp_untptr_t value);
lisp_ptr_tag_t get_ptr_tag(lisp_ptr_t ptr);
lisp_untptr_t  get_ptr_content(lisp_ptr_t ptr);
lisp_untptr_t  fix_numeric_content(u32int ptr_content);

#endif
