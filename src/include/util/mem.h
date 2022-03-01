#pragma once

#include <util/types.h>

inline void memset(void *dst, u8 value, size_t n) {
    u8 *d = dst;

    while (n-- > 0) {
        *d++ = value;
    }
}

inline void *memcpy(void *dst, const void *src, size_t n) {
    u8 *d = dst;
    const u8 *s = src;

    while (n-- > 0) {
        *d++ = *s++;
    }

    return d;
}

inline void *memmove(void *dst, const void *src, size_t n) {
    // OK since we know that memcpy copies forwards
    if (dst < src) {
        return memcpy(dst, src, n);
    }

    u8 *d = dst;
    const u8 *s = src;

    for (size_t i = n; i > 0; i--) {
        d[i - 1] = s[i - 1];
    }

    return dst;
}

//https://code.woboq.org/gcc/libiberty/memcmp.c.html
inline int memcmp (const void* str1, const void* str2, size_t count)
{
  register const unsigned char *s1 = (const unsigned char*)str1;
  register const unsigned char *s2 = (const unsigned char*)str2;
  while (count-- > 0)
    {
      if (*s1++ != *s2++)
          return s1[-1] < s2[-1] ? -1 : 1;
    }
  return 0;
}
