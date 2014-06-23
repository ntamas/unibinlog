/* vim:set ts=4 sw=4 sts=4 et: */

#include <stdarg.h>
#include <stdio.h>

#include <unibinlog/debug.h>

unsigned short int __ub_debug_mode = 0;

void ub_set_debug_mode(unsigned short int on) {
    __ub_debug_mode = !!on;
}

void UB_WARNING(const char* component, const char* format, ...) {
    va_list args;
    va_start(args, format);
    if (component != 0) {
        fprintf(stderr, "[%s] ", component);
    }
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}

