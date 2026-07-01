#include <kernel/utils/bitmap.h>
#include <kernel/error.h>
#include <stdint.h>

size_t bitmap_find(bitmap_t *bitmap, size_t bitmap_len, size_t min, size_t max, bool set, size_t *out_len) {
    size_t free_index = 0;
    size_t free_count = 0;

    bitmap_t full_element = set ? 0 : BITMAP_T_MAX;
    bitmap_t empty_element = ~full_element;

    for (size_t i = 0; i < bitmap_len && free_count < max; i++) {
        bitmap_t element = bitmap[i];

        if (element == full_element) {
            if (free_count >= min) {
                break;
            }
            free_count = 0;
            continue;
        }

        if (element == empty_element) {
            if (free_count == 0) {
                free_index = i * BITMAP_BITS_PER_ELEMENT;
            }
            free_count += BITMAP_BITS_PER_ELEMENT;
            continue;
        }

        bool sequence_break = false;
        for (unsigned char j = __builtin_ctzl(set ? element : ~element); j < BITMAP_BITS_PER_ELEMENT && free_count < max; j++) {
            // [TODO] optimize: not necessary for the first iteration
            bool bit = bitmap_get_ex(bitmap, i, j);
            if (set ? !bit : bit) {
                if (free_count >= min) {
                    sequence_break = true;
                    break;
                }
                free_count = 0;
                continue;
            }
                
            if (free_count == 0) {
                free_index = i * BITMAP_BITS_PER_ELEMENT + j;
            }
            free_count++;
        }
        if (sequence_break) {
            break;
        }
    }
    
    ASSERT(free_count >= min, { if (out_len != NULL) *out_len = 0; return SIZE_MAX; });

    if (out_len != NULL) {
        *out_len = free_count;
    }
    return free_index;
}
