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

    cdef struct Xcfa:
        get_lexp_type get_lexp
        one_step_type one_step
        char * description
    ctypedef Xcfa * cfa

    cfa get_cfa_from_name(char * name)
    char ** list_of_algorithms()
    unsigned int number_of_algorithms()

cdef extern from "mcf_algorithms.h":
    void brun_mf_get_lexp(double *, double *, unsigned int)
    void brun_mf_one_step(Point3d p)

    void jacobi_perron_get_lexp(double *, double *, unsigned int)
    void jacobi_perron_one_step(Point3d p)

cdef class MCFAlgorithm(object):
    cdef get_lexp_type _get_lexp
    cdef one_step_type _one_step
    cdef str _short_name
    cdef str _description
