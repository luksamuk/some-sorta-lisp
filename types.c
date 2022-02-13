#include "types.h"

const char *
type_repr_str(datatype_t type)
{
    switch(type) {
    case TYPE_UNDEFINED:        return "UNDEF";
    case TYPE_CONS:             return "CONS ";
    case TYPE_ATOM:             return "ATOM ";
    case TYPE_NUMBER:           return "NUM  ";
    case TYPE_BUILTIN_FUNCTION: return "CFUNC";
    case TYPE_BUILTIN_SPECIAL:  return "CSPCL";
    case TYPE_FUNCTION:         return "FUNCT";
    case TYPE_SPECIAL:          return "SPECL";
    default:                    return "WTF  ";
    }
}

