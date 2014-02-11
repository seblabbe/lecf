/* (Multplicative floor) Selmer algorithm        */
/* simplex: x < y < z and z < x + y              */
/* algo   : (x,y,z) -> sort(x, y, z - x)         */
/* dual   : (u,v,w) -> (u+w, v, w) + permutation */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME selmer
#define ALGORITHM_FULL_NAME Selmer (multi)
#define NB_EXTREMAL_PTS 3
#define EXTREMAL_PTS 1,1,2, 0,1,1, 1,1,1
#endif

#ifdef ALGORITHM
z -= x;
u += w;
if(z > x) {PERMUTE_xzy;}   // x,y,z = x,z,y   (and same for dual)
else {PERMUTE_zxy;}        // x,z,y = z,x,y   (and same for dual)
#endif
