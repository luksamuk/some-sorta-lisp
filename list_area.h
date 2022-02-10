#ifndef LISP_LIST_AREA_H
#define LISP_LIST_AREA_H

#include "types.h"

list_area_t    make_list_area();
void           print_list_area(list_area_t *area);
void           clear_list_area(list_area_t *area);
lisp_untptr_t  make_cons_cell(list_area_t *area, lisp_ptr_t car, lisp_ptr_t cdr);

#endif
