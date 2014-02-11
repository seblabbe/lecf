/* Algorithm of Baladi and Nogueira                  */
/* It is a Poincare nearest followed by one step  of */
/* additive Arnoux-Rauzy.                            */
/* simplex: x < y,z                                  */
/* (x,y,z) -> (x, y - mx, z - nx) or                 */
/*            (x, (m+1)x- y, z - nx) or              */
/*            (x, y - mx, (n+1)x - z) or             */
/*            (x, (m+1)x - y, (n+1)x - z)            */
/* composed with (x,y,z) -> (x - y - z, y, z)        */

#ifdef PREAMBLE
#define ALGORITHM_SHORT_NAME baladi_nogueira
#define ALGORITHM_FULL_NAME Baladi-Nogueira
#define NB_EXTREMAL_PTS 3
#define EXTREMAL_PTS 0,0,1, 0,1,0, 1,1,1
#endif

#ifdef EXTRA_VARIABLES
double m;
#endif

#ifdef ALGORITHM
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

/* now put the smallest in first place */
if(x > y)
{
	if(y > z) {PERMUTE_zxy;}
	else {PERMUTE_yxz;}
}
else if(x > z) {PERMUTE_zxy;}
#endif
