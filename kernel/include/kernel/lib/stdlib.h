#ifndef _STDLIB_H
#define _STDLIB_H 1

#ifdef __cplusplus
extern "C"
{
#endif

__attribute__((__noreturn__)) void abort(void);
char* lutoa(unsigned long num, char* str, int base);

#ifdef __cplusplus
}
#endif

#endif
