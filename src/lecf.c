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

#define XAPPEND_TO_NAME(X,Y) X ## Y
#define APPEND_TO_NAME(X,Y) XAPPEND_TO_NAME(X,Y)

#define ONE_STEP APPEND_TO_NAME(ALGORITHM_SHORT_NAME, _one_step)
#define GET_LEXP APPEND_TO_NAME(ALGORITHM_SHORT_NAME, _get_lexp)
#define RANDOM_POINT APPEND_TO_NAME(ALGORITHM_SHORT_NAME, _random_point)
#define DOMAIN APPEND_TO_NAME(ALGORITHM_SHORT_NAME, _domain_extremal_pts)
#define DOMAIN_NB APPEND_TO_NAME(ALGORITHM_SHORT_NAME, _domain_pts_nb)

#define PREAMBLE
#include ALGO_INCLUDE
#undef PREAMBLE

/* macro for sorting and permutations of coordinates           */
/* Warning: as you may notice, this needs an extra variables s */
#define SORT_xyz         \
if(y > z)                \
{                        \
	s=z; z=y; y=s;       \
	s=w; w=v; v=s;       \
}                        \
if(x > z)                \
{                        \
	s=x; x=y; y=z; z=s;  \
	s=u; u=v; v=w; w=s;  \
}                        \
else if(x > y)           \
{                        \
	s=y; y=x; x=s;       \
	s=v; v=u; u=s;       \
}

#define PERMUTE_yxz {\
s=x; x=y; y=s;      \
s=u; u=v; v=s;}

#define PERMUTE_zyx {\
s=x; x=z; z=s;      \
s=u; u=w; w=s;}

#define PERMUTE_xzy {\
s=y; y=z; z=s;      \
s=v; v=w; w=s;}

#define PERMUTE_yzx {\
s=x; x=y; y=z; z=s;  \
s=u; u=v; v=w; w=s;} \

#define PERMUTE_zxy {\
s=x; x=z; z=y; y=s;  \
s=u; u=w; w=v; v=s;}

const double DOMAIN[] = {EXTREMAL_PTS};
const unsigned int DOMAIN_NB = NB_EXTREMAL_PTS;

Point3d RANDOM_POINT(void)
{
	double ratios[NB_EXTREMAL_PTS];
	double s;
	unsigned int i;
	Point3d p;

	for(i=0; i <NB_EXTREMAL_PTS; ++i) ratios[i] = drand();

	p = Point3d_new();
	p->x = p->y = p->z = 0;
	for(i=0; i<NB_EXTREMAL_PTS; ++i)
	{
		p->x += ratios[i] * DOMAIN[3*i];
		p->y += ratios[i] * DOMAIN[3*i+1];
		p->z += ratios[i] * DOMAIN[3*i+2];
	}
	s = p->x + p->y + p->z;
	p->x /= s; p->y /= s; p->z /= s;

	p->u = drand(); p->v = drand(); p->w = drand();
	s = p->x * p->u + p->y * p->v + p->z * p->w;
	p->u -= s; p->v -= s; p->w -= s;
	s = fabs(p->u) + fabs(p->v) + fabs(p->w);
	p->u /= s; p->v /= s; p->w /= s;

	return p;
}

void ONE_STEP(Point3d P)
{
	double s;
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

bool GET_LEXP(double *lexp1, double *lexp2, unsigned int nb_iterations)
{
	Point3d p;
	double x,y,z,u,v,w;           /*(x,y,z) and (u,v,w) are the two vectors used */
	double s;                     /* temporary double variables  */
	int i;                        /* loop counter */
	double theta1=0, theta2=0;    /* values of Lyapunov exponents */
	bool error = false;

	#define EXTRA_VARIABLES
	#include ALGO_INCLUDE
	#undef EXTRA_VARIABLES

	p = RANDOM_POINT();
	x = p->x; y = p->y; z = p->z;
	u = p->u; v = p->v; w = p->w;
	Point3d_free(&p);

#ifdef VERBOSE
	printf("u=%f, v=%f, w=%f\n",u,v,w);
	s = x*u + y*v + z*w;
	printf("scal prod <(x,y,z),(u,v,w)> = %f\n",s);
#endif

	i = 0;
	while(i < nb_iterations && !error)
	{
        /* apply the algorithm on (x,y,z) and apply the dual */
		/* transformation on (u,v,w).                        */
#ifdef ORDERED
		while(z > CRITICAL_VALUE && !error)
#else
		while(((x > CRITICAL_VALUE) || (y > CRITICAL_VALUE) || (z > CRITICAL_VALUE)) && !error)
#endif
		{
#define ALGORITHM
#include ALGO_INCLUDE
#undef ALGORITHM
			++i;

#ifdef ASSERT
			if((x <= 0) || (y <= 0) || (z <= 0))
			{
                /* occurs to often: pollutes the screen
				fprintf(stderr, "Error: after %d-th application of algorithm\n",i);
				fprintf(stderr, "Error: one of the length is <=0: x=%f, y=%f, z=%f\n",x,y,z);
                */
				error = true;
			}
			s = x*u + y*v + z*w;
			if ((s > 0.0001) || (s < -0.0001))
			{
			    fprintf(stderr, "Error: after %d-th application of algorithm\n",i);
			    fprintf(stderr, "Error: scal prod <(x,y,z),(u,v,w)> =%f is not zero\n",s);
			    error = true;
			}
#endif
		}

		/* renormalization (using Kahan summation algorithm) */
		/* does not depend on the cf algorithm               */
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
		u /= s; v /= s; w /= s;

		/* the following orthogonalization seems useless, but it is not!!! */
		s = x*u + y*v + z*w;
		u -= s; v -= s; w -= s;

#ifdef ASSERT
		if ((x < CRITICAL_VALUE) && (y  < CRITICAL_VALUE) && (z < CRITICAL_VALUE))
		{
		    fprintf(stderr, "Error: After %d-th application of the algorithm.\n",i);
		    fprintf(stderr, "Error: The renormalization on the vector did not worked.\n");
		    fprintf(stderr, "Error: We stop otherwise, we get an infinite loop.\n");
		    error = true;
		}
#endif
	}
	if (error)
	{
    /* occurs to often: pollutes the screen
	fprintf(stderr, "Error: expect bad computation of Lyapuvov exp. for this experiment\n");
    */
	}

	*lexp1 = theta1 / nb_iterations;
	*lexp2 = theta2 / nb_iterations;
	return error;
}
