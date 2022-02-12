#ifndef LISP_LIST_AREA_H
#define LISP_LIST_AREA_H

#include "types.h"

void           init_list_area(list_area_t *area_ptr);
void           print_list_area(list_area_t *area);
lisp_untptr_t  make_cons_cell(list_area_t *area, lisp_ptr_t car, lisp_ptr_t cdr);
void           cell_set_car(list_area_t *area, lisp_untptr_t ptr, lisp_ptr_t value);
void           cell_set_cdr(list_area_t *area, lisp_untptr_t ptr, lisp_ptr_t value);
lisp_ptr_t     get_car(list_area_t *area, lisp_untptr_t ptr);
lisp_ptr_t     get_cdr(list_area_t *area, lisp_untptr_t ptr);

#endif
