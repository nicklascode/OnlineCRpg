#include "logger.h"

static void log_message(const char* prefix, const char* fmt, va_list args) {
    printf("%s: ", prefix);
    vprintf(fmt, args);
    printf("\n");
}

void DEBUG_LOG(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message("DEBUG", fmt, args);
    va_end(args);
}

void ERROR_LOG(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message("ERROR", fmt, args);
    va_end(args);
}
