/*
**  LTOA.C
**
**  Converts a long integer to a string.
**
**  Copyright 1988-90 by Robert B. Stout dba MicroFirm
**
**  Released to public domain, 1991
**
**  Parameters: 1 - number to be converted
**              2 - buffer in which to build the converted string
**              3 - number base to use for conversion
**
**  Returns:  A character pointer to the converted string if
**            successful, a NULL pointer if the number base specified
**            is out of range.
*/

#include <stdlib.h>
#include <string.h>

#define BUFSIZE (sizeof(long int) * 8 + 1)

char *ltoa(long int n, char *str, int base)
{
    int i = 2;
    long int uarg;
    char *tail, *head = str, buf[BUFSIZE];

    if (36 < base || 2 > base)
        base = 10;            /* can only use 0-9, A-Z        */
    tail = &buf[BUFSIZE - 1]; /* last character position      */
    *tail-- = '\0';

    if (10 == base && n < 0L)
    {
        *head++ = '-';
        uarg = -n;
    }
    else
        uarg = n;

    if (uarg)
    {
        for (i = 1; uarg; ++i)
        {
            // register ldiv_t r;

            // r = ldiv(uarg, base);
            long int rem = uarg % base;
            long int quot = uarg / base;
            *tail-- = (char)(rem + ((9L < rem) ? ('A' - 10L) : '0'));
            uarg = quot;
        }
    }
    else
        *tail-- = '0';

    memcpy(head, ++tail, i);
    return str;
}
