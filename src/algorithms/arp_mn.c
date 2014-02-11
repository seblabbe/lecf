/* (nearest) Arnoux-Rauzy Poincaré algorithm                  */
/* simplex: x < y < z                                         */
/* algo   : (x,y,z) -> sort(x, y,     z - m(x - y) ) if z > x + y */
/*                     sort(x, y - x, z - y)     otherwise    */
/* dual   : (u,v,w) -> (u+m*w, v+m*w, w) + permutation */
/*                     (u+v+w, v+w, w)   + permutation */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME arp_mn
#define ALGORITHM_FULL_NAME (Multi nearest) ARP
#define NB_EXTREMAL_PTS 3
#define EXTREMAL_PTS 0,0,1, 0,1,1, 1,1,1
#endif

#ifdef EXTRA_VARIABLES
double m;
#endif


#ifdef ALGORITHM
if (z > x+y){

m = round(z / (x+y));
z -= m * (x+y);
u += m*w;
v += m*w;

if(z < 0)
{
	z = -z;
	w = -w;
}

} else {

z -= y;
y -= x;
v += w;
u += v;   // note that this v includes the w
}

SORT_xyz;
#endif

