#ifndef LISP_CONSTANTS_H
#define LISP_CONSTANTS_H

/// Possible types for typed pointers.
/// Should have the size of an uint8_t.
enum DATATYPE {
    TYPE_UNDEFINED        = 0x0,
    TYPE_CONS             = 0x1,
    TYPE_ATOM             = 0x8,
    TYPE_NUMBER           = 0x9,
    TYPE_BUILTIN_FUNCTION = 0xa,
    TYPE_BUILTIN_SPECIAL  = 0xb,
    TYPE_FUNCTION         = 0xc,
    TYPE_SPECIAL          = 0xd,
    TYPE_LAMBDA_FUNCTION  = 0xe,
    TYPE_LAMBDA_SPECIAL   = 0xf,
};

enum CONSTANT_ATOM {
    A_NIL     = 0x000000,
    A_T       = 0x000001,
    A_QUOTE   = 0x000002,
    A_ATOM    = 0x000003,
    A_EQ      = 0x000004,
    A_CAR     = 0x000005,
    A_CDR     = 0x000006,
    A_CONS    = 0x000007,
    A_COND    = 0x000008,
    A_LAMBDA  = 0x000009,
    A_LABEL   = 0x00000A,
    A_SPECIAL = 0x00000B,
    A_UNDEF   = 0x00000C,
};


enum CONSTANT_TYPED_POINTER {
    TP_NIL     = 0x08000000,
    TP_T       = 0x08000001,
    TP_QUOTE   = 0x08000002,
    TP_ATOM    = 0x08000003,
    TP_EQ      = 0x08000004,
    TP_CAR     = 0x08000005,
    TP_CDR     = 0x08000006,
    TP_CONS    = 0x08000007,
    TP_COND    = 0x08000008,
    TP_LAMBDA  = 0x08000009,
    TP_LABEL   = 0x0800000A,
    TP_SPECIAL = 0x0800000B,
    TP_UNDEF   = 0x0800000C,
};

#endif
