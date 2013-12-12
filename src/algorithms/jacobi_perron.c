/* Jacobi-Perron algorithm */
/* simplex: x,y < z                          */
/* algo   : (x,y,z) -> (y-mx, z-nx, x)       */
/* dual   : (u,v,w) -> (v, w, u + m*v + n*w) */

#ifdef PREAMBLE
#define ALGORITHM_SHORT_NAME jacobi_perron
#define ALGORITHM_FULL_NAME Jacobi-Perron
#endif

#ifdef EXTRA_VARIABLES
double m,n;
#endif

#ifdef ALGORITHM
s = x;
m = floor(y / s);
n = floor(z / s);
x = y - m*s;
y = z - n*s;
z = s;

s = w;
w = u + m*v + n*w;
u = v;
v = s;
#endif
