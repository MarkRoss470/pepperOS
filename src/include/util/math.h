#ifndef MATH_H
#define MATH_H

#include <util/types.h>

#define E 2.71828
#define PI 3.14159265358979323846264338327950

f64 fmod(f64 x, f64 m);
f64 fabs(f64 x);
f64 sin(f64 x);
f64 cos(f64 x);
f64 pow(f64 x, f64 y);
inline i64 abs(i64 x)
{
	return((x > 0) ? x : -x);
}
inline u64 mul64(u64 a, u64 b)
{
	u32 aLow = (u32) a;
	u32 aHigh = (u32) (a >> 32);
	u32 bLow = (u32) b;
	u32 bHigh = (u32) (b >> 32);
	//no multiplication of aHigh with bHigh as it would result in an overflow, so no point calculating
	return (u64)(aLow * aHigh) + ((u64)(aLow * bHigh + bLow * aHigh) << 32);
}

//I don't know what this does and that this point I'm to afraid to ask
//https://github.com/glitchub/arith64/blob/master/arith64.c
inline int __clzdi2(u64 a)
{
    int b, n = 0;
    b = !(a & 0xffffffff00000000ULL) << 5; n += b; a <<= b;
    b = !(a & 0xffff000000000000ULL) << 4; n += b; a <<= b;
    b = !(a & 0xff00000000000000ULL) << 3; n += b; a <<= b;
    b = !(a & 0xf000000000000000ULL) << 2; n += b; a <<= b;
    b = !(a & 0xc000000000000000ULL) << 1; n += b; a <<= b;
    return n + !(a & 0x8000000000000000ULL);
}

inline u64 div64(u64 a, u64 b)
{
	if(b == 0){volatile char x = 0; x = 1 / x;}
	if(b > a)return 0;
	if(b == 1)return a;
	if(!(a >> 32 && b >> 32))return (u32)a / (u32)b;
	

	// let's do long division
	char bits = __clzdi2(b) - __clzdi2(a) + 1;  // number of bits to iterate (a and b are non-zero)
	u64 rem = a >> bits;                   // init remainder
	a <<= 64 - bits;                            // shift numerator to the high bit
	u64 wrap = 0;                          // start with wrap = 0
	while (bits-- > 0)                          // for each bit
	{
	rem = (rem << 1) | (a >> 63);           // shift numerator MSB to remainder LSB
	a = (a << 1) | (wrap & 1);              // shift out the numerator, shift in wrap
	wrap = ((i64)(b - rem - 1) >> 63);  // wrap = (b > rem) ? 0 : 0xffffffffffffffff (via sign extension)
	rem -= b & wrap;                        // if (wrap) rem -= b
	}
	return (a << 1) | (wrap & 1);               // return the quotient
	
}

#endif
