#ifndef _MCF_TEMPLATE_H
#define _MCF_TEMPLATE_H
#include "point3d.h"

typedef enum { false, true } bool;

typedef bool (*get_lexp_type)(double * lexp1, double *, unsigned int nb_iterations);
typedef void (*one_step_type)(Point3d);
typedef Point3d (*random_point_type)(void);

typedef struct Xcfa
{
	get_lexp_type get_lexp;
	one_step_type one_step;
	random_point_type random_point;
	char * description;
	double * extremal_pts;
	unsigned int extremal_pts_nb;
} * cfa;

cfa get_cfa_from_name(char * name);
char ** list_of_algorithms(void);
unsigned int number_of_algorithms(void);
#endif
