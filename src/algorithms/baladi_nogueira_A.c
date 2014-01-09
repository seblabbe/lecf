/* Algorithm of Baladi and Nogueira                  */
/* It is a joacobi nearest followed by one step  of  */
/* additive Arnoux-Rauzy.                            */
/* simplex: x < y < 0.5  and z is whatever           */

#ifdef PREAMBLE
#define ALGORITHM_SHORT_NAME baladi_nogueira_A
#define ALGORITHM_FULL_NAME Baladi-Nogueira Algo A
#define NB_EXTREMAL_PTS 3
#define EXTREMAL_PTS 0,0,1, 1,1,0, 0,1,1
#endif

#ifdef EXTRA_VARIABLES
double m;
#endif

#ifdef ALGORITHM
if (x < z) {
m = roundf(y/x);
y -= m*x;
u += m*v;
if(y < 0) {y = -y; v = -v;}

m = roundf(z/x);
z -= m*x;
u += m*w;
if(z < 0) {z = -z; w = -w;}

x -= y; x -= z;
v += u;
w += u;

if (y < z) {
    s = x;
    x = y;
    y = z;
    z = s;
    s = u;
    u = v;
    v = w;
    w = s;
} else {
    s = x;
    x = z;
    z = s;
    s = u;
    u = w;
    w = s;
}

} else { /* z < x */

m = roundf(y/z);
y -= m*z;
w += m*v;
if(y < 0) {y = -y; v = -v;}

m = roundf(x/z);
x -= m*z;
w += m*u;
if(x < 0) {x = -x; u = -u;}

z -= y; z -= x;
v += w;
u += w;

if (x > y) {
    s = x;
    x = y;
    y = s;
    s = u;
    u = v;
    v = s;
}
}

#endif
