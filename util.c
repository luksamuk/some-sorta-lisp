#include "def.h"

void
dbg(const char *format, ...)
{
/*#ifdef NDEBUG
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
//#else
    UNUSED(format);
//#endif*/
}

void
edbg(const char *format, ...)
{
/*#if defined(NDEBUG) && defined(EDEBUG)
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
//#else
    UNUSED(format);
//#endif*/
}
