/* (Multplicative floor) Selmer algorithm        */
/* simplex: x < y < z and z < x + y              */
/* algo   : (x,y,z) -> sort(x, y, z - x)         */
/* dual   : (u,v,w) -> (u+w, v, w) + permutation */

#ifdef PREAMBLE
#define ORDERED
#define ALGORITHM_SHORT_NAME selmer
#define ALGORITHM_FULL_NAME (multiplicative floor) Selmer
#endif

#ifdef PREPROCESS
/* by default, the initial value belongs to the simplex  */
/* x < y < z. As Selmer has an extra condition, we first */
/* refined to that smaller simplex                       */
while(z > x + y)
{
z -= x;
}
#endif

#ifdef EXTRA_VARIABLES
#endif

#ifdef ALGORITHM
#ifdef ASSERT
if(z > x+y) fprintf(stderr, "Error in Selmer z > x+y\n");
#endif
z = z - x;
u = u + w;
#ifdef ASSERT
if(z > y) fprintf(stderr, "Error in Selmer x > y");
#endif
if(z > x)
{
	s = z; t = w;
	z = y; w = v;
	y = s; v = t;
}
else
{
	s = z; t = w;
	z = y; w = v;
	y = x; v = u;
	x = s; u = t;
}
#endif
