#include "pointers.h"

lisp_ptr_t
make_pointer(lisp_ptr_tag_t tag, uint32_t value)
{
    uint32_t clean_value = value ^ (value & LISP_PTR_TAG_CLEAR_MASK);
    if(tag & TYPE_NUMBER != 0) {
        // Compensate signum
        uint32_t signum = value >> 31;
        clean_value |= (signum << 19);
    }
    uint32_t padded_tag = tag;
    padded_tag = padded_tag << 24;
    return padded_tag | clean_value;
}

lisp_ptr_tag_t
get_ptr_tag(lisp_ptr_t ptr)
{
    return ptr >> 24;
}

uint32_t
get_ptr_content(lisp_ptr_t ptr)
{
    return ptr ^ (ptr & LISP_PTR_TAG_CLEAR_MASK);
}

uint32_t
fix_numeric_content(lisp_ptr_t ptr_content)
{
    // FIXME: This is incorrect and won't work properly.
    uint32_t signum = ptr_content >> 23;
    ptr_content ^= 0x800000;
    ptr_content |= signum << 31;
    return ptr_content;
}

