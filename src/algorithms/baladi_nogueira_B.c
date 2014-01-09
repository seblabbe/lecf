/* Algorithm B of Baladi and Nogueira                */
/* It is kind of a Brun nearest followed by one step */
/* of additive Arnoux-Rauzy.                         */
/* simplex: x < y < z                                */
/* (this should look like algo B)                    */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME baladi_nogueira_B
#define ALGORITHM_FULL_NAME Baladi-Nogueira Algo B
#define NB_EXTREMAL_PTS 3
#define EXTREMAL_PTS 0,0,1, 0,1,1, 1,1,1
#endif

#ifdef EXTRA_VARIABLES
double m;
#endif

#ifdef ALGORITHM
m = round(z/y);
z -= m*y;
v += m*w;
if(z < 0) {z = -z; w = -w;}

m = round(x/y);   /* m takes value 0 or 1*/
x -= m*y;
v += m*u;
if(x < 0) {x = -x; u = -u;}

y -= z; y -= x;
w += v;
u += v;

SORT_xyz;
#endif
