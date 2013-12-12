/* (Multplicative floor) Brun algorithm                               */
/* The coordinates are ordered x < y < z                              */
/* (x,y,z) -> reorder(x, y, z - my) where m = floor(z/y)              */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME brun_mf
#define ALGORITHM_FULL_NAME (multiplicative floor) Brun
#define NB_EXTREMAL_PTS 3
#define EXTREMAL_PTS 0,0,1, 0,1,1, 1,1,1
#endif

#ifdef EXTRA_VARIABLES
double n;
#endif

#ifdef ALGORITHM
n = floor(z / y);
z -= n*y;
v += n*w;

if(z > x) {PERMUTE_xzy;}   // x,y,z = x,z,y   (and same for dual)
else {PERMUTE_zxy;}        // x,y,z = z,x,y   (and same for dual)
#endif
