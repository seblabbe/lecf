#include "random.h"

inline double drand()
{
	return (((double) rand()) + (((double) rand())/((double)RAND_MAX))) / ((double) RAND_MAX);
}


