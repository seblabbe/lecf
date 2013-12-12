/* (Multplicative nearest) Brun algorithm */
/* The coordinates are ordered x < y < z  */
/* (x,y,z), then take z' = min(z - my, (m+1)y - z) and then reorder */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME brun_mn
#define ALGORITHM_FULL_NAME (multiplicative nearest) Brun
#endif

#ifdef EXTRA_VARIABLES
#endif

#ifdef ALGORITHM
s = roundf(z / y);
z -= s*y;
v += s*w;
if(z < 0)
{
	z = -z;
	w = -w;
}

PERMUTE_xyz
#endif
