/* (Multplicative nearest) Brun algorithm */
/* The coordinates are ordered x < y < z  */
/* (x,y,z), then take z' = min(z - my, (m+1)y - z) and then reorder */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME brun_mn
#define ALGORITHM_FULL_NAME Brun (multi nearest)
#define NB_EXTREMAL_PTS 3
#define EXTREMAL_PTS 0,0,1, 0,1,1, 1,1,1
#endif

#ifdef EXTRA_VARIABLES
double n;
#endif

#ifdef ALGORITHM
n = roundf(z / y);
z -= n*y;
v += n*w;
if(z < 0)
{
	z = -z;
	w = -w;
}

if(x < z) {PERMUTE_xzy;}   // x,y,z = x,z,y   (and same for dual)
else {PERMUTE_zxy;}        // x,y,z = z,x,y   (and same for dual)
#endif
