#ifndef KERNEL_UTILS_BITMAP
#define KERNEL_UTILS_BITMAP

#include <limits.h>
#include <stddef.h>
#include <stdbool.h>

typedef unsigned long bitmap_t;

#define BITMAP_BITS_PER_ELEMENT (sizeof(bitmap_t) * CHAR_BIT)
#define BITMAP_INDEX(bit) ((bit) / BITMAP_BITS_PER_ELEMENT)
#define BITMAP_OFFSET(bit) ((bit) % BITMAP_BITS_PER_ELEMENT)
#define BITMAP_ELEMENTS(n) (((n) + BITMAP_BITS_PER_ELEMENT - 1) / BITMAP_BITS_PER_ELEMENT)
#define BITMAP_T_MAX ULONG_MAX

static inline void bitmap_set_ex(bitmap_t* bitmap, size_t index, unsigned char offset) {
    bitmap[index] |= (1UL << offset);
}

static inline void bitmap_set(bitmap_t* bitmap, size_t bit) {
    bitmap_set_ex(bitmap, BITMAP_INDEX(bit), BITMAP_OFFSET(bit));
}

static inline void bitmap_clear_ex(bitmap_t* bitmap, size_t index, unsigned char offset) {
    bitmap[index] &= ~(1UL << offset); 
}

static inline void bitmap_clear(bitmap_t* bitmap, size_t bit) {
    bitmap_clear_ex(bitmap, BITMAP_INDEX(bit), BITMAP_OFFSET(bit));
}

static inline bool bitmap_get_ex(bitmap_t* bitmap, size_t index, unsigned char offset) {
    return (bitmap[index] & (1UL << offset)) != 0;
}

static inline bool bitmap_get(bitmap_t* bitmap, size_t bit) {
    return bitmap_get_ex(bitmap, BITMAP_INDEX(bit), BITMAP_OFFSET(bit));
}

static inline unsigned char bitmap_ctz(bitmap_t element) {
    return (unsigned char)(__builtin_ctzl(element));
}

#endif
