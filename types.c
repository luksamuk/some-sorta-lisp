#include "types.h"

const char *
type_repr_str(datatype_t type)
{
    switch(type) {
    case TYPE_CONS:             return "CONS ";
    case TYPE_UNDEFINED:        return "UNDEF";
    case TYPE_VARIABLE:         return "VAR  ";
    case TYPE_NUMBER:           return "NUM  ";
    case TYPE_BUILTIN_FUNCTION: return "BIFN ";
    case TYPE_BUILTIN_SPECIAL:  return "BISPL";
    case TYPE_FUNCTION:         return "FUNC ";
    case TYPE_SPECIAL:          return "SPCL ";
    case TYPE_LAMBDA_FUNCTION:  return "ULFN ";
    case TYPE_LAMBDA_SPECIAL:   return "ULSPL";
    default:                    return "WTF  ";
    }
}

