#include "mcf_algorithms.h"

cfa get_cfa_from_name(char * name)
{
	unsigned int i;
	cfa c;

	for(i=0; i<NB_ALGORITHMS; ++i)
	{
		if(strcmp(algo_names[i], name) == 0)
		{
			c = (cfa) malloc(sizeof(struct Xcfa));
			c->get_lexp = get_lexps[i];
			c->one_step = one_steps[i];
			c->description = descriptions[i];
			return c;
		}
	}
	return NULL;
}

unsigned int number_of_algorithms(void)
{
	return NB_ALGORITHMS;
}

char ** list_of_algorithms(void)
{
	return algo_names;
}


