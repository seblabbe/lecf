/* (Multplicative floor) Brun algorithm */
/* The coordinates are ordered x > y > z  */
/* (x,y,z) -> reorder(x - my, y, z) where m = floor(x/y) */

#ifdef PREAMBLE
#define PERMUTE
#define ALGORITHM_NAME "(multiplicative floor) Brun"
#endif

#ifdef EXTRA_VARIABLES
#endif

#ifdef ALGORITHM
s = floor(x / y);
x = x - s*y;
v = v + s*u;
#endif
