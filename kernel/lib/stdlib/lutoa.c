#include <kernel/lib/stdlib.h>
#include <kernel/lib/string.h>

/**
 * Converts an unsigned long to a string.
 * @param num  The number to convert.
 * @param str  The buffer to store the result.
 * @param base The numerical base (e.g., 10 for decimal, 16 for hex).
 * @return     A pointer to the resulting string.
 */
char* lutoa(unsigned long num, char* str, int base) {
    char* ptr = str;
    char* start = str;
    unsigned long remainder;

    // Standard guard for invalid bases
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    // Process individual digits
    do {
        remainder = num % base;
        // Convert remainder to character: '0'-'9' or 'a'-'z'
        *ptr++ = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'a');
        num /= base;
    } while (num > 0);

    // Terminate the string
    *ptr = '\0';

    // The digits were added in reverse order, so we swap them
    char* end = ptr - 1;
    char temp;
    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    return str;
}   