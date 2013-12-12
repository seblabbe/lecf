#ifndef _POINT3D_H
#define _POINT3D_H
#include "random.h"
#include <stdlib.h>
#include <stdio.h>

struct XPoint3d {
    double x,y,z,u,v,w;
};
typedef struct XPoint3d * Point3d;

Point3d Point3d_new(void);
void Point3d_free(Point3d *p);
Point3d Point3d_copy(Point3d p);

inline void Point3d_sort(Point3d p);
/* sort such that x < y < z */

inline void Point3d_set_random(Point3d p);
inline void Point3d_randomize(Point3d p);

#endif
