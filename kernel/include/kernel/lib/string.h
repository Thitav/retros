#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Copies a block of memory from a source address to a destination address.
   *
   * @param dstptr Pointer to the destination address.
   * @param srcptr Pointer to the source address.
   * @param size   Number of bytes to be copied.
   * @return       Pointer to the destination address `dstptr`.
   */
  void *memcpy(void *__restrict dstptr, const void *__restrict srcptr, size_t size);

  /**
   * Moves a block of memory from a source address to a destination address, even if the memory blocks overlap.
   *
   * @param dstptr Pointer to the destination address.
   * @param srcptr Pointer to the source address.
   * @param size   Number of bytes to be moved.
   * @return       Pointer to the destination address `dstptr`.
   */
  void *memmove(void *dstptr, const void *srcptr, size_t size);

  /**
   * Sets a block of memory with a specified value.
   *
   * @param bufptr Pointer to the memory block.
   * @param value  Value to be set.
   * @param size   Number of bytes to be set.
   * @return       Pointer to the memory block `bufptr`.
   */
  void *memset(void *bufptr, int value, size_t size);

  /**
   * Compares two memory blocks.
   *
   * @param aptr Pointer to the first memory block.
   * @param bptr Pointer to the second memory block.
   * @param size Number of bytes to be compared.
   * @return     An integer less than, equal to, or greater than zero \
   * if the first block is found, respectively, to be less than, \
   * to match, or be greater than the second block.
   */
  int memcmp(const void *aptr, const void *bptr, size_t size);

  /**
   * Calculates the length of a null-terminated string.
   *
   * @param str Pointer to the string.
   * @return    Length of the string.
   */
  size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif
