#ifndef COMPRESS_UTIL_H
#define COMPRESS_UTIL_H

#include <stddef.h>
#include <stdint.h>

unsigned char* compress_data(const unsigned char* data, int dataSize, int* compDataSize);
unsigned char* decompress_data(const unsigned char* compData, int compDataSize, int* dataSize);

#endif // COMPRESS_UTIL_H
