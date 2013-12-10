/* (Multplicative floor) Selmer algorithm */
/* The coordinates are ordered x > y > z  */
/* and must satisfy x < y + z             */
/* (x,y,z) -> reorder(x - z, y, z)        */

#ifdef PREAMBLE
#define ALGORITHM_NAME "(multiplicative floor) Selmer"
#endif

#ifdef PREPROCESS
while(x > y + z)
{
	x -= z;
}
#endif

#ifdef EXTRA_VARIABLES
#endif

#ifdef ALGORITHM
#ifdef ASSERT
if(x > y+z)
{
	fprintf(stderr, "Error 1 in Selmer at step %d\n",i);
	exit(EXIT_FAILURE);
}
#endif
x = x - z;
w = w + u;
#ifdef ASSERT
if(x > y)
{
	fprintf(stderr, "Error 2 in Selmer at step %d\n",i);
	exit(EXIT_FAILURE);
}
#endif
if(x > z)
{
	s = x; t = u;
	x = y; u = v;
	y = s; v = t;
}
else
{
	s = x; t = u;
	x = y; u = v;
	y = z; v = w;
	z = s; w = t;
}
#endif
