/*
 * util.h - utility functions
 */

#include "streamtypes.h"

#ifndef _UTIL_H
#define _UTIL_H

/* host endian independent multi-byte integer reading */

static inline int16_t get_16bitBE(uint8_t * p) {
    return (p[0]<<8) | (p[1]);
}

static inline int16_t get_16bitLE(uint8_t * p) {
    return (p[0]) | (p[1]<<8);
}

static inline int32_t get_32bitBE(uint8_t * p) {
    return (p[0]<<24) | (p[1]<<16) | (p[2]<<8) | (p[3]);
}

static inline int32_t get_32bitLE(uint8_t * p) {
    return (p[0]) | (p[1]<<8) | (p[2]<<16) | (p[3]<<24);
}

void put_8bit(uint8_t * buf, int8_t i);

void put_16bitLE(uint8_t * buf, int16_t i);

void put_32bitLE(uint8_t * buf, int32_t i);

void put_16bitBE(uint8_t * buf, int16_t i);

void put_32bitBE(uint8_t * buf, int32_t i);

/* signed nibbles come up a lot */
static int nibble_to_int[16] = {0,1,2,3,4,5,6,7,-8,-7,-6,-5,-4,-3,-2,-1};

static inline int get_high_nibble_signed(uint8_t n) {
    /*return ((n&0x70)-(n&0x80))>>4;*/
    return nibble_to_int[n>>4];
}

static inline int get_low_nibble_signed(uint8_t n) {
    /*return (n&7)-(n&8);*/
    return nibble_to_int[n&0xf];
}

/* return true for a good sample rate */
int check_sample_rate(int32_t sr);

/* return a file's extension (a pointer to the first character of the
 * extension in the original filename or the ending null byte if no extension
 */
const char * filename_extension(const char * filename);

static inline int clamp16(int32_t val) {
        if (val>32767) return 32767;
            if (val<-32768) return -32768;
                return val;
}

void swap_samples_le(sample *buf, int count);

void concatn(int length, char * dst, const char * src);


/* Simple stdout logging for debugging and regression testing purposes.
 * Needs C99 variadic macros. */
#ifdef VGM_DEBUG_OUTPUT
/* equivalent to printf when condition is true */
#define VGM_ASSERT(condition, ...) \
    do { if (condition) printf(__VA_ARGS__); } while (0)
/* equivalent to printf */
#define VGM_LOG(...) \
    do { printf(__VA_ARGS__); } while (0)
/* prints file/line/func */
#define VGM_LOGF() \
    do { printf("%s:%i '%s'\n",  __FILE__, __LINE__, __func__); } while (0)
/* prints a buffer/array */
#define VGM_LOGB(buf, buf_size, bytes_per_line) \
    do { \
        int i; \
        for (i=0; i < buf_size; i++) { \
            printf("%02x",buf[i]); \
            if (bytes_per_line && (i+1) % bytes_per_line == 0) printf("\n"); \
        } \
        printf("\n"); \
    } while (0)
#else

#define VGM_ASSERT(condition, ...) /* nothing */
#define VGM_LOG(...) /* nothing */
#define VGM_LOGF() /* nothing */
#define VGM_LOGB(buf, buf_size, bytes_per_line) /* nothing */

#endif

#endif
