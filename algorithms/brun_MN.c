/* (Multplicative nearest) Brun algorithm */
/* The coordinates are ordered x > y > z  */
/* (x,y,z), then take x' = min(x - my, (m+1)y - x) and then reorder */

#ifdef PREAMBLE
#define PERMUTE
#define ALGORITHM_NAME "(multiplicative nearest) Brun"
#endif

#ifdef EXTRA_VARIABLES
#endif

#ifdef ALGORITHM
s = roundf(x / y);
x = x - s*y;
v = v + s*u;
if(x < 0)
{
	x = -x;
	u = -u;
}
#endif
