#ifndef MATH_H
#define MATH_H

#include "types.h"

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

#endif
