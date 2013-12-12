/* Jacobi-Perron algorithm */
/* simplex: x,y < z                          */
/* algo   : (x,y,z) -> (y-mx, z-nx, x)       */
/* dual   : (u,v,w) -> (v, w, u + m*v + n*w) */

#ifdef PREAMBLE
#define ALGORITHM_SHORT_NAME jacobi_perron
#define ALGORITHM_FULL_NAME Jacobi-Perron
#define NB_EXTREMAL_PTS 4
#define EXTREMAL_PTS 0,0,1, 1,0,1, 1,1,1, 0,1,1
#endif

#ifdef EXTRA_VARIABLES
double r,m,n;
#endif

#ifdef ALGORITHM
r = x;
m = floor(y / r);
n = floor(z / r);
x = y - m*r;
y = z - n*r;
z = r;

r = w;
w = u + m*v + n*w;
u = v;
v = r;
#endif
