cdef extern from "point3d.h":
    cdef struct XPoint3d:
        double x,y,z,u,v,w
    ctypedef XPoint3d * Point3d

    Point3d Point3d_new()
    void Point3d_free(Point3d *p)
    inline void Point3d_set_random(Point3d p)
    inline void Point3d_sort(Point3d p)
    inline void Point3d_copy(Point3d p)

cdef extern from "mcf_template.h":
    ctypedef void (*get_lexp_type)(double * lexp1, double *, unsigned int nb_iterations)
    ctypedef void (*one_step_type)(Point3d)
    ctypedef Point3d (*random_point_type)()

    cdef struct Xcfa:
        get_lexp_type get_lexp
        one_step_type one_step
        random_point_type random_point
        char * description
        double * extremal_pts
        unsigned int extremal_pts_nb
    ctypedef Xcfa * cfa

    cfa get_cfa_from_name(char * name)
    char ** list_of_algorithms()
    unsigned int number_of_algorithms()

cdef class MCFAlgorithm(object):
    cdef get_lexp_type _get_lexp
    cdef one_step_type _one_step
    cdef random_point_type _random_point
    cdef double * _extremal_pts
    cdef unsigned int _extremal_pts_nb
    cdef str _short_name
    cdef str _description
