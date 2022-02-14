#pragma once

#include "types.h"
#include "mem.h"

inline size_t strlen(const char *str) 
{
    size_t l = 0;
    while (*str++ != 0) {
        l++;
    }
    return l;
}

inline char *itob(i32 x, char *s, size_t sz) 
{
	for(size_t i = 0; i < sz; i++)
	{
		s[i] = ((x & (1 << i))>>i) + 0x30;
	}
	return(s);
}

inline char *itoa(i32 x, char *s, size_t sz)
 {
    // TODO: holy god this is bad code we need some error handling here
    if (sz < 20) {
        void panic(const char *);
        panic("ITOA BUFFER TOO SMALL");
    }

    u32 tmp;
    i32 i, j = 0;

    tmp = x;
    i = 0;
    if(x < 0)
    {
    	s[0] = '-';
    	j++;
    	i++;
    	x = -x;
    }
    do {
        tmp = x % 10;
        s[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (x /= 10);
    s[i--] = 0;

    for (; j < i; j++, i--) {
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }

    return s;
}

inline char *uitoa(u32 x, char *s, size_t sz)
 {
    // TODO: holy god this is bad code we need some error handling here
    if (sz < 20) {
        void panic(const char *);
        panic("ITOA BUFFER TOO SMALL");
    }

    u32 tmp;
    i32 i, j = 0;

    tmp = x;
    i = 0;
    
    do {
        tmp = x % 10;
        s[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (x /= 10);
    s[i--] = 0;

    for (; j < i; j++, i--) {
        tmp = s[j];
        s[j] = s[i];
        s[i] = tmp;
    }

    return s;
}

inline char *itohex(i64 x, char *s, size_t sz, int c)
{
    // TODO: holy god this is bad code we need some error handling here
    if (sz < 10) {
        void panic(const char *);
        panic("ITOH BUFFER TOO SMALL");
    }
    const char hex_chars[2][17] = {"0123456789ABCDEF", "0123456789abcdef"};
    bool first_char = false;
    int j = 0;
    for(int i = 0; i < 16; i++)
    {
    	unsigned int char_int = (x >> (60 - 4 * i)) & 0xf;
    	if (char_int != 0 || first_char || i == 15)
    	{
    		s[j] = hex_chars[c][char_int];
    		first_char = true;
    		j++;
    	}
    }
    s[j] = '\0';
    return s;
}

inline char *itoh(i64 x, char *s, size_t sz)
{
	return(itohex(x, s, sz, 1));
}

inline char *ITOH(i64 x, char *s, size_t sz)
{
	return(itohex(x, s, sz, 0));
}

// SEE: https://opensource.apple.com/source/Libc/Libc-1158.30.7/string/strlcat.c.auto.html
inline size_t strlcat(char *dst, const char *src, size_t size)
{
    const size_t sl = strlen(src),
          dl = strlen(dst);

    if (dl == size) {
        return size + sl;
    }

    if (sl < (size - dl)) {
        memcpy(dst + dl, src, sl + 1);
    } else {
        memcpy(dst + dl, src, size - dl - 1);
        dst[size - 1] = '\0';
    }

    return sl + dl;
}

inline size_t strlcpy(char *dst, const char *src, size_t n)
{
    // copy as many bytes as can fit
    char *d = dst;
    const char *s = src;
    size_t size = n;

    while (--n > 0) {
        if ((*d++ = *s++) == 0) {
            break;
        }
    }

    // if we ran out of space, null terminate
    if (n == 0) {
        if (size != 0) {
            *d = 0;
        }

        // traverse the rest of s
        while (*s++);
    }

    return s - src - 1;
}

inline bool strcmp(const char* str1, const char* str2)
{
	int i = 0;
	while(1)
	{
		if(str1[i] != str2[i]){return(false);}
		if(str1[i] == '\0' && str2[i] == '\0'){return(true);}
		i++;
	}
}

inline int strncmp(const char *s1, const char *s2, register size_t n)
{
  register unsigned char u1, u2;
  while (n-- > 0)
    {
      u1 = (unsigned char) *s1++;
      u2 = (unsigned char) *s2++;
      if (u1 != u2)
        return u1 - u2;
      if (u1 == '\0')
        return 0;
    }
  return 0;
}

inline int uatoi(const char *s)
{
	int total = 0;
	for(int i = 0; s[i] != '\0'; i++)
	{
		if(s[i] > '9' || s[i] < '0')return -1;
		total *= 10;
		total += s[i] - '0';
	}
	return total;
}



