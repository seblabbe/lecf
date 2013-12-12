#include "mcf_algorithms.h"

int main(int argc, char **argv)
{
	unsigned int i;
	unsigned int nb_iterations = 100000;
	unsigned int nb_experiments = 100;
	unsigned int seed = (unsigned) time(NULL);

	double *lexp1, *lexp2;
	double mean,mean1,mean2,stddev,stddev1,stddev2;
	int opt;

	char algorithm_name[64];
	strcpy(algorithm_name, "brun_mf");

	void (*get_lexp)(double *,double *,unsigned int);
	get_lexp = &(brun_mf_get_lexp);

	while((opt = getopt(argc, argv, "a:e:i:s:")) != -1)
	{
		switch(opt)
		{
			case 'a':
				// choice of the algorithm
				if(strcmp(optarg,"brun_mf") == 0)
				{
					strcpy(algorithm_name, "brun_mf");
					get_lexp = &(brun_mf_get_lexp);
				}
				else if(strcmp(optarg,"brun_mn") == 0)
				{
					strcpy(algorithm_name, "brun_mn");
					get_lexp = &(brun_mn_get_lexp);
				}
				else if(strcmp(optarg,"jacobi_perron") == 0)
				{
					strcpy(algorithm_name, "jacobi_perron");
					get_lexp = &(jacobi_perron_get_lexp);
				}
				else if(strcmp(optarg,"selmer") == 0)
				{
					strcpy(algorithm_name, "selmer");
					get_lexp = &(selmer_get_lexp);
				}
				else
				{
					fprintf(stderr, "Algorithms should be one of: brun_mf, brun_mn, jacobi_perron, selmer\n");
					exit(EXIT_FAILURE);
				}
				break;
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

	printf("algorithm: %s\n", algorithm_name);
	printf("nb iterations: %u\n", nb_iterations);
	printf("nb experiments: %u\n", nb_experiments);
	printf("seed: %u\n", seed);
	fflush(stdout);

	srand(seed);

	lexp1 = (double *) malloc(nb_experiments * sizeof(double));
	lexp2 = (double *) malloc(nb_experiments * sizeof(double));

	for(i=0; i<nb_experiments; ++i)
	{
		(*get_lexp)(lexp1 + i, lexp2 + i, nb_iterations);
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

