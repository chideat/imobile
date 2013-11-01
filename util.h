#ifndef UTILS_H
#define UTILS_H

#ifndef uint32
#define uint32 unsigned int
#endif

#ifndef uint64
#define uint64 unsigned long long
#endif

#ifndef uint16
#define uint16 unsigned short
#endif

#ifndef uint8
#define uint8 unsigned char
#endif

#ifndef uchar
#define uchar uint8
#endif

#ifndef int32
#define int32 int
#endif

#ifndef int64
#define int64 long long
#endif

#ifndef int16
#define int16 short
#endif

#ifndef int8 
#define int8 char
#endif

#define MAX_BUF 8192

#include "ConvertUTF.h"
#include <string.h>

static void convert_utf8_to_utf16(char *source, uint32 len_s, char **target, uint32 *len_t) {
    const UTF8 *sourceStart = (UTF8 *)source;
    UTF8 *sourceEnd = (UTF8 *)(source + len_s);
    UTF16 *targetStart, *targetEnd;
    *len_t = len_s * 2;
    *target = new char[*len_t + 1];
    memset(*target, 0, *len_t + 1);
    targetStart = (UTF16 *)*target;
    targetEnd = (UTF16 *)(*target + *len_t);

    if (conversionOK != ConvertUTF8toUTF16(&sourceStart, sourceEnd, &targetStart, targetEnd, strictConversion) ) {
#ifdef DEBUG
        fprintf(stderr, "Convert to UTF-16 failed\n");
#endif
        throw 1;
    }
    *len_t = (uint32)((char *)targetStart - *target);
}

static uint64 *get64uint(char *cts, int seek) {
    return (uint64 *)(cts + seek);
}

static uint32 *get32uint(char *cts, int seek) {
    return (uint32 *)(cts + seek);
}

static int32 *get32int(char *cts, int seek) {
    return (int32 *)(cts + seek);
}

static uint16 *get16uint(char *cts, int seek) {
    return (uint16 *)(cts + seek);
}

static int16 *get16int(char *cts, int seek) {
    return (int16 *)(cts + seek);
}

static uint8 *get8uint(char *cts, int seek) {
    return (uint8 *)(cts + seek);
}

static int8 *get8int(char *cts, int seek) {
    return (int8 *)(cts + seek);
}

static float *get32float(char *cts, int seek) {
    return (float *)(cts + seek);
}

#endif
