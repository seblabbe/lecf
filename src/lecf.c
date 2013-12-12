/* parameters (might be modified) */

/* perform some check and randomize the values */
#define ASSERT
#define RANDOMIZE

/* subtle parameter... */
#ifndef CRITICAL_VALUE
#define CRITICAL_VALUE  .0001
#endif

/* from here: DO NOT TOUCH! */
/* some black magic preprocessor !! */
/* make a nice ALGO_INCLUDE from a dirty ALGO_FILE */
#define MAKESTR(x) #x
#define XMAKESTR(x) MAKESTR(x)
#define ALGO_INCLUDE XMAKESTR(ALGO_FILE)

#define XXONE_STEP(X) X ## _one_step
#define XONE_STEP(X) XXONE_STEP(X)
#define ONE_STEP XONE_STEP(ALGORITHM_SHORT_NAME)

#define XXGET_LEXP(X) X ## _get_lexp
#define XGET_LEXP(X) XXGET_LEXP(X)
#define GET_LEXP XGET_LEXP(ALGORITHM_SHORT_NAME)

#define PREAMBLE
#include ALGO_INCLUDE
#undef PREAMBLE

/* macro for permutations x < y < z */
#define PERMUTE_xyz            \
if(y > z)                      \
{                              \
	s=z; t=w;                  \
	z=y; w=v;                  \
	y=s; v=t;                  \
}                              \
if(x > z)                      \
{                              \
	s=x; t=u;                  \
	x=y; u=v;                  \
	y=z; v=w;                  \
	z=s; w=t;                  \
}                              \
else if(x > y)                 \
{                              \
	s=y; t=v;                  \
	y=x; v=u;                  \
	x=s; u=t;                  \
}

void ONE_STEP(Point3d P)
{
	double s,t;
	#define EXTRA_VARIABLES
	#include ALGO_INCLUDE
	#undef EXTRA_VARIABLES

	#define ALGORITHM
	#define x (P->x)
	#define y (P->y)
	#define z (P->z)
	#define u (P->u)
	#define v (P->v)
	#define w (P->w)
	#include ALGO_INCLUDE
	#undef x
	#undef y
	#undef z
	#undef u
	#undef v
	#undef w
	#undef ALGORITHM
}

void GET_LEXP(double *lexp1, double *lexp2, unsigned int nb_iterations)
{
	double x,y,z,u,v,w;           /*(x,y,z) and (u,v,w) are the two vectors used */
	double s,t;                   /* temporary double variables  */
	int i;                        /* loop counter */
	double theta1=0, theta2=0;    /* values of Lyapunov exponents */

	#define EXTRA_VARIABLES
	#include ALGO_INCLUDE
	#undef EXTRA_VARIABLES

	x = drand(); y = drand(); z = drand();
	u = drand() - .5; v = drand() - .5; w = drand() - .5;

	#ifdef ORDERED
	/* order and normalize (x,y,z) to get */
	/*  1) 0 < x < y < z                  */
	/*  2) x + y + z = 1                  */
	if(y > z) {t=z; z=y; y=t;}
	if(x > z) {t=x; x=y; y=z; z=t;}
	else if(x > y) {t=y; y=x; x=t;}
	#endif

	#define PREPROCESS
	#include ALGO_INCLUDE
	#undef PREPROCESS

	s = x+y+z;
	x /= s; y /= s; z /= s;

	/* normalize (u,v,w) */
	t = x*u + y*v + z*w;
	s = x*x + y*y + z*z;
	u -= t*x/s; v -= t*y/s; w -= t*z/s;
	s = fabs(u) + fabs(v) + fabs(w);  /* WARNING: abs is for int and fabs for double !!! */
	u /= s; v /= s; w /= s;

#ifdef VERBOSE
	printf("u=%f, v=%f, w=%f\n",u,v,w);
	s = x*u + y*v + z*w;
	printf("scal prod <(x,y,z),(u,v,w)> = %f\n",s);
#endif

	i = 0;
	while(i < nb_iterations)
	{
        /* apply the algorithm on (x,y,z) and apply the dual */
		/* transformation on (u,v,w).                        */
#ifdef ORDERED
		while(z > CRITICAL_VALUE)
#else
		while((x > CRITICAL_VALUE) || (y > CRITICAL_VALUE) || (z > CRITICAL_VALUE))
#endif
		{
#define ALGORITHM
#include ALGO_INCLUDE
#undef ALGORITHM
			++i;

#ifdef ASSERT
			if((x <= 0) || (y <= 0) || (z <= 0))
			{
				fprintf(stderr, "Error: after %d-th application of algorithm\n",i);
				fprintf(stderr, "Error: one of the length is 0: x=%f, y=%f, z=%f",x,y,z);
				exit(EXIT_FAILURE);
			}
			s = x*u + y*v + z*w;
			if ((s > 0.0001) || (s < -0.0001))
			{
			    fprintf(stderr, "Error: after %d-th application of algorithm\n",i);
			    fprintf(stderr, "Error: scal prod <(x,y,z),(u,v,w)> =%f is not zero\n",s);
			    exit(EXIT_FAILURE);
			}
#endif


#if VERBOSE > 2
			printf("\nx=%f, y=%f, z=%f\n",x,y,z);
			printf("u=%f, v=%f, w=%f\n",u,v,w);
			s = x*u + y*v + z*w;
			printf("scal prod after permutation <(x,y,z),(u,v,w)> = %f\n",s);
#endif
#ifdef ASSERT
			s = x*u + y*v + z*w;
			if ((s > 0.0001) || (s < -0.0001))
			{
			    fprintf(stderr, "Error: After %d-th application of algorithm\n",i);
			    fprintf(stderr, "Error: After permutation of entries\n");
			    fprintf(stderr, "Error: scal prod <(x,y,z),(u,v,w)> =%f is not zero\n",s);
			    exit(EXIT_FAILURE);
			}
#endif
		}

		/* renormalization (using Kahan summation algorithm) */
		/* does not depend on the cf algorithm               */
#ifdef VERBOSE
		printf("Renormalization after %d-th application of algorithm\n",i);
		printf("x=%f, y=%f, z=%f\n",x,y,z);
		printf("u=%f, v=%f, w=%f\n",u,v,w);
#endif

		s = x+y+z;
		theta1 -= log(s);

		/* a bit of salt? */
#ifdef RANDOMIZE
		x /= s; y /= s; z /= s;
		x += drand() / 100000000000;
		y += drand() / 100000000000;
		z += drand() / 100000000000;
		s = x + y + z;
#endif
		x /= s; y /= s;	z /= s;

		s = fabs(u) + fabs(v) + fabs(w);
		theta2 += log(s);

		/* the following gramm shimdts seems to be useless, but it is not!!! */
		t = x*u + y*v + z*w;
		s = x*x + y*y + z*z;
		u -= t*x/s; v -= t*y/s; w -= t*z/s;
		s = fabs(u) + fabs(v) + fabs(w);
		u /= s; v /= s; w /= s;

#ifdef VERBOSE
		printf("x=%f, y=%f, z=%f\n",x,y,z);
		printf("u=%f, v=%f, w=%f\n",u,v,w);
#endif

#ifdef ASSERT
		if ((x < CRITICAL_VALUE) && (y  < CRITICAL_VALUE) && (z < CRITICAL_VALUE))
		{
		    fprintf(stderr, "Error: After %d-th application of the algorithm.\n",i);
		    fprintf(stderr, "Error: The renormalization on the vector did not worked.\n");
		    fprintf(stderr, "Error: We stop otherwise, we get an infinite loop.\n");
		    exit(EXIT_FAILURE);
		}
#endif
	}

	*lexp1 = theta1 / nb_iterations;
	*lexp2 = theta2 / nb_iterations;
}
