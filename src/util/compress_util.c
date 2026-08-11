#include "compress_util.h"
#include "../raylib_m.h"

unsigned char* compress_data(const unsigned char* data, int dataSize, int* compDataSize) {
    return CompressData(data, dataSize, compDataSize);
}

unsigned char* decompress_data(const unsigned char* compData, int compDataSize, int* dataSize) {
    return DecompressData(compData, compDataSize, dataSize);
}
