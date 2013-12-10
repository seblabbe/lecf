/* Jacobi-Perron algorithm */

#ifdef PREAMBLE
#define ALGORITHM_NAME "Jacobi-Perron"
#endif

#ifdef EXTRA_VARIABLES
double m,n;
double r;
#endif

#ifdef ALGORITHM
m = floor(x / z);
n = floor(y / z);
r = x - m*z;
s = y - n*z;
t = z;

x = t;
y = r;
z = s;

r = u;
s = v;
t = m*u + n*v + w;

u = t;
v = r;
w = s;
#endif
