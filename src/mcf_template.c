#include "mcf_algorithms.h"
#include "algorithms_array.c"

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
			c->description = full_names[i];
			c->random_point = random_points[i];
			c->extremal_pts = extremal_pts + 3*extremal_pts_bounds[i];
			c->extremal_pts_nb = extremal_pts_nb[i];
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
	return (char **) algo_names;
}


