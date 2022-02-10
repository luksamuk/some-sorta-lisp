#include <stdarg.h>
#include <stdio.h>

void
dbg(const char *format, ...)
{
#ifdef NDEBUG
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
#endif
}
