/* (Multplicative floor) Brun algorithm                               */
/* The coordinates are ordered x < y < z                              */
/* (x,y,z) -> reorder(x, y, z - my) where m = floor(z/y)              */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME brun_mf
#define ALGORITHM_FULL_NAME (multiplicative floor) Brun
#endif

#ifdef ALGORITHM
s = floor(z / y);
z -= s*y;
v += s*w;

PERMUTE_xyz
#endif
