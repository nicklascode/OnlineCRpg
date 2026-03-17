#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

void DEBUG_LOG(const char* fmt, ...);
void ERROR_LOG(const char* fmt, ...);

#endif // LOGGER_H
