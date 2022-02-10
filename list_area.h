#ifndef LISP_LIST_AREA_H
#define LISP_LIST_AREA_H

#include "types.h"

#define LISP_PTR_TAG_CLEAR_MASK 0xFF000000

list_area_t    make_list_area();
void           print_list_area(list_area_t *area);
lisp_ptr_t     make_pointer(lisp_ptr_tag_t tag, uint32_t value);
void           clear_list_area(list_area_t *area);

lisp_ptr_tag_t get_ptr_tag(lisp_ptr_t ptr);
uint32_t       get_ptr_content(lisp_ptr_t ptr);
uint32_t       fix_numeric_content(uint32_t ptr_content);

lisp_untptr_t  make_cons_cell(list_area_t *area, lisp_ptr_t car, lisp_ptr_t cdr);

#endif
