/* Arnoux-Rauzy Poincaré algorithm                            */
/* simplex: x < y < z                                         */
/* algo   : (x,y,z) -> sort(x, y,     z - x - y) if z > x + y */
/*                     sort(x, y - x, z - y)     otherwise    */
/* dual   : (u,v,w) -> (u+w,   v+w, w) + permutation */
/*                     (u+v+w, v+w, w) + permutation */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME arp
#define ALGORITHM_FULL_NAME Arnoux-Rauzy Poincaré algorithm
#define NB_EXTREMAL_PTS 3
#define EXTREMAL_PTS 0,0,1, 0,1,1, 1,1,1
#endif

#ifdef ALGORITHM
if (z > .5){
z -= x + y;
u += w;
v += w;
} else {
z -= y;
y -= x;
v += w;
u += v;   // no that this v includes the w
}

SORT_xyz;
#endif
