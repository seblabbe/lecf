#include "point3d.h"

Point3d Point3d_new(void)
{
	return (Point3d) malloc(sizeof(struct XPoint3d));
}

void Point3d_free(Point3d *p)
{
	free(*p);
	*p = NULL;
}

Point3d Point3d_copy(Point3d p)
{
	Point3d q = Point3d_new();
	q->x = p->x; q->y = p->y; q->z = p->z;
	q->u = p->u; q->v = p->v; q->w = p->w;
	return q;
}

inline void Point3d_sort(Point3d p)
{
	double s;

	if(p->y > p->z)
	{
		s = p->y;
		p->y = p->z;
		p->z = s;

		/* dual */
		s = p->v;
		p->v = p->w;
		p->w = s;
	}
	if(p->x > p->z)
	{
		s = p->x;
		p->x = p->y;
		p->y = p->z;
		p->z = s;

		/* dual */
		s = p->u;
		p->u = p->v;
		p->v = p->w;
		p->w = s;
	}
	else if(p->x > p->y)
	{
		s = p->x;
		p->x = p->y;
		p->y = s;

		/* dual */
		s = p->u;
		p->u = p->v;
		p->v = s;
	}
}

inline void Point3d_set_random(Point3d p)
{
	double s;

	p->x = drand(); p->y = drand(); p->z = drand();
	s = p->x + p->y + p->z;
	p->x /= s; p->y /= s; p->z /= s;

	p->u = drand(); p->v = drand(); p->w = drand();
	s = p->x * p->u + p->y * p->v + p->z * p->w;
	p->u -= s;
	p->v -= s;
	p->w -= s;
}

inline void Point3d_randomize(Point3d p)
{
	double s = p->x + p->y + p->z;
	double t;

	p->x += s * ((drand() - .5) / 1000000000);
	p->y += s * ((drand() - .5) / 1000000000);
	p->z += s * ((drand() - .5) / 1000000000);

	if( (p->x < 0) || (p->y < 0) || (p->z < 0))
	{
		fprintf(stderr, "trouble with randomize\n");
	}

	s = p->x + p->y + p->z;
	t = p->x * p->u + p->y * p->v + p->z * p->w;
	p->u -= t/s;
	p->v -= t/s;
	p->w -= t/s;
}
