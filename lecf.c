#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


// some black magic preprocessor !!

// choose a default algorithm if not overriden with -D compiler option
#ifndef ALGO_FILE
#define ALGO_FILE algorithms/jacobi_perron.c
#endif

// make a nice input string out of the name
#define MAKESTR(x) #x
#define XMAKESTR(x) MAKESTR(x)
#define ALGO_INCLUDE XMAKESTR(ALGO_FILE)

#define PREAMBLE
#include ALGO_INCLUDE
#undef PREAMBLE

/* perform some check */
#define ASSERT
#define RANDOMIZE

#define CRITICAL_VALUE  .0001

void get_lexp(double *theta1, double *theta2, unsigned int nb_experiments);

int main(int argc, char **argv)
{
	unsigned int i;
	unsigned int nb_iterations = 100000;
	unsigned int nb_experiments = 100;
	unsigned int seed = (unsigned) time(NULL);

	double *lexp1, *lexp2;
	double mean,mean1,mean2,stddev,stddev1,stddev2;
	int opt;

	while((opt = getopt(argc, argv, "e:i:s:")) != -1)
	{
		switch(opt)
		{
			case 'e':
				nb_experiments = (unsigned int) atol(optarg);
				break;
			case 'i':
				nb_iterations = (unsigned int) atol(optarg);
				break;
			case 's':
				seed = (unsigned int) atol(optarg);
				break;
			default:
				fprintf(stderr, "Usage: %s [-e nb_experiments] [-i nb_iterations] [-s seed]\n",argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	printf("algorithm: %s\n", ALGORITHM_NAME);
	printf("nb iterations: %u\n", nb_iterations);
	printf("nb experiments: %u\n", nb_experiments);
	printf("seed: %u\n", seed);
	fflush(stdout);

	srand(seed);

	lexp1 = (double *) malloc(nb_experiments * sizeof(double));
	lexp2 = (double *) malloc(nb_experiments * sizeof(double));

	for(i=0; i<nb_experiments; ++i)
	{
		get_lexp(lexp1 + i, lexp2 + i, nb_iterations);
		mean1 += lexp1[i];
		mean2 += lexp2[i];
		mean += lexp2[i] / lexp1[i];
	}
	mean1 /= nb_experiments;
	mean2 /= nb_experiments;
	mean /= nb_experiments;

	/* now compute mean and standard deviation */
	for(i=0; i<nb_experiments; ++i)
	{
		stddev1 += (lexp1[i] - mean1) * (lexp1[i] - mean1);
		stddev2 += (lexp2[i] - mean2) * (lexp2[i] - mean2);
		stddev  += (lexp2[i]/lexp1[i] - mean) * (lexp2[i]/lexp1[i] - mean);
	}
	stddev1 = sqrt(stddev1/(nb_experiments - 1));
	stddev2 = sqrt(stddev2/(nb_experiments - 1));
	stddev  = sqrt(stddev/(nb_experiments - 1));

	printf("top L. exp: %f   (%f)\n", mean1, stddev1);
	printf("scd L. exp: %f   (%f)\n", mean2, stddev2);
	printf("ratio     : %f   (%f)\n", mean, stddev);

	free(lexp1);
	free(lexp2);

	return 0;

}

inline double drand()
{
	return (((double) rand()) + (((double) rand())/((double)RAND_MAX))) / ((double) RAND_MAX);
}


void get_lexp(double *lexp1, double *lexp2, unsigned int nb_iterations)
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

	/* order and normalize (x,y,z) to get */
	/*  1) x > y > z > 0                  */
	/*  2) x + y + z = 1                  */
	if(y > x) {t=x; x=y; y=t;}
	if(z > x) {t=z; z=y; y=x; x=t;}
	else if(z > y) {t=y; y=z; z=t;}

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
#ifdef PERMUTE
		while(x > CRITICAL_VALUE)
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

#ifdef PERMUTE
			/* permute to get x > y > z */
			if(y > x)
			{
				s=x; t=u;
				x=y; u=v;
				y=s; v=t;
			}
			if(z > x)
			{
				s=z; t=w;
				z=y; w=v; 
				y=x; v=u;
				x=s; u=t;
			}
			else if(z > y)
			{
				s=y; t=v;
				y=z; v=w;
				z=s; w=t;
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
