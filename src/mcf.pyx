r"""
Multidimensional Continued Fraction Algorithms

Gain d'optimisation in Cython:

    - Declare x,y,z as double instead of list : factor of 4
    - Faire les calculs dans une boucle : factor of 2
    - Do not use yield : factor of 10

TODO:

    - reuteneaour, nogueira, autres?
    - make a other class for 2d and and 1d methods

Question:

    - Comment factoriser le code sans utiliser les yield?
    - Comment faire un appel de fonction rapide (pour factoriser le code)

[1] https://groups.google.com/forum/?fromgroups=#!topic/sage-devel/NCBmj2KjwEM

AUTHORS:

 - Vincent Delecroix, C code, Computation of Lyapounov exponents for Brun
   algorithm, June 2013.
 - Sebastien Labbe, Invariant measures, Lyapounov exponents and natural
   extensions for a dozen of algorithms, October 2013.

"""
from libc.stdlib cimport malloc, free
from libc.string cimport memset
import numpy

def check_all():
    r"""
    Check all algorithms available.
    """
    for algo in algorithm_names():
        print "check %s..."%algo,
        c = MCFAlgorithm(algo, check=False)
        c._check()
        c._check()
        c._check()
        print "done"

def project2d(p):
    r"""
    Project the point ``p`` (with three cordinates) into the plane.

    TODO: it is stupid to recompute cos(2*pi/3) and sin(2*pi/3) each time the
    function is called...
    """
    from math import cos,sin,pi
    c0 = 1.; s0 = 0.
    c1 = cos(2*pi/3); s1 = sin(2*pi/3)
    c2 = cos(-2*pi/3); s2 = sin(-2*pi/3)

    return (p[0]*c0 + p[1]*c1 + p[2]*c2, p[0]*s0 + p[1]*s1 + p[2]*s2)

def get_side(a, b):
    r"""
    routine for ``inside_convex_polygon``
    """
    x = a[0]*b[1] - a[1]*b[0]
    if x < 0:
        return 0
    elif x > 0:
        return 1
    else:
        return None

def inside_convex_polygon(point, vertices):
    r"""
    Check whether ``point`` is inside the convex hull of ``vertices``.

    TODO: implement it in C directly in lekz.c in order to provide further
    tests when ASSERT is set.
    """
    previous_side = None
    n_vertices = len(vertices)
    for n in xrange(n_vertices):
        a, b = vertices[n], vertices[(n+1)%n_vertices]
        affine_segment = (b[0]-a[0], b[1]-a[1])
        affine_point = (point[0]-a[0], point[1]-a[1])
        current_side = get_side(affine_segment, affine_point)
        if current_side is None:
            return False #outside or over an edge
        elif previous_side is None: #first segment
            previous_side = current_side
        elif previous_side != current_side:
            return False
    return True

def algorithm_names():
    cdef char ** algos = list_of_algorithms()
    l = []
    for i in xrange(number_of_algorithms()):
        l.append(str(algos[i]))
    return l

cdef class MCFAlgorithm(object):
    def __init__(self, name=None, check=True):

        assert isinstance(name, str)
        cdef cfa c = get_cfa_from_name(name)
        if c is NULL:
            raise ValueError("no algorithm named %s. Must be one of: %s"%(name, ', '.join(algorithm_names())))

        self._short_name = name
        self._description = c.description
        self._get_lexp = c.get_lexp
        self._one_step = c.one_step
        self._random_point = c.random_point
        self._extremal_pts = c.extremal_pts
        self._extremal_pts_nb = c.extremal_pts_nb

        free(c)

        if check:
            self._check()

    def domain_extremal_pts(self):
        cdef unsigned int i
        result = []
        for i in xrange(self._extremal_pts_nb):
            result.append((self._extremal_pts[3*i],
                           self._extremal_pts[3*i+1],
                           self._extremal_pts[3*i+2]))
        return result

    def _check(self):
        r"""
        Do some check to test consistency of the algorithm.

        More precisely, the code pick 100 random points and check if they belong
        to the domain. Pick a random point and iterates 1000 times the algorithm
        and check.
        """
        cdef Point3d P
        cdef double s,t
        cdef int i

        dom = map(project2d, self.domain_extremal_pts())

        for i in range(100):
            P = self._random_point()
            s = P.x * P.u + P.y * P.v + P.z * P.w
            if s < -0.000001 or s > 0.000001:
                raise AssertionError("scalar product is not 0 for random point =%s"%((P.x,P.y,P.z),))
            if not inside_convex_polygon(project2d((P.x, P.y, P.z)), dom):
                raise AssertionError("random point (=%s) not inside the domain"%((P.x, P.y, P.z),))

            Point3d_free(&P)

        P = self._random_point()
        import sys
        sys.stdout.flush()

        for i in range(1000):
            self._one_step(P)
            if not inside_convex_polygon(project2d((P.x,P.y,P.z)), dom):
                raise AssertionError("point (=%s) out of the domain after %d steps"%((P.x,P.y,P.z),i))
            s = P.x * P.u + P.y * P.v + P.z * P.w
            if P.x <= 0 or P.y <= 0 or P.z <= 0:
                raise AssertionError("one of the coordinate become null after %d steps"%i)
            if s < -0.000001 or s > 0.000001:
                raise AssertionError("orthogonality not preserved by the algo after %d steps (scal=%f)"%(i,s))

            s = P.x + P.y + P.z
            P.x /= s; P.y /= s; P.z /= s

            s = P.x * P.u + P.y * P.v + P.z * P.w
            P.u -= s; P.v -= s; P.w -= s

        for _ in range(10):
            P = self._random_point()
            P.u = 0.3
            P.v = 0.3
            P.w = 0.3
            s = P.x * P.u + P.y * P.v + P.z * P.w
            self._one_step(P)
            t = P.x * P.u + P.y * P.v + P.z * P.w
            if not (-0.00001 < s - t < 0.00001) :
                raise AssertionError("scalar product not preserved: %f != %f" % (s,t))

        Point3d_free(&P)

    def name(self):
        return self._short_name

    def long_name(self):
        return self._description

    def info(self):
        r"""
        Return some information about the algorithm.
        """
        raise NotImplementedError

    def __repr__(self):
        return self.long_name()

    def lyapunov_exponents(self,
            unsigned int n_iterations=10000,
            unsigned int n_experiments=100,
            verbose=True):
        r"""
        INPUT:

        - ``n_iterations`` -- integer
        - ``n_experiments`` -- integer
        -- ``verbose`` -- boolean (default: ``True``)
        """
        cdef double *theta1, *theta2
        cdef unsigned int i
        cdef double m1,m2,m
        cdef double stddev1,stddev2,stddev

        assert n_iterations > 100, "n_iterations (=%s) must be more than 100" % n_iterations
        assert n_experiments > 1, "n_experiments (=%s) must be more than 1" % n_experiments

        theta1 = <double *> malloc(n_experiments * sizeof(double))
        theta2 = <double *> malloc(n_experiments * sizeof(double))

        theta1_list = []
        theta2_list = []
        ratio_list = []

        m = m1 = m2 = 0
        for i in range(n_experiments):
            error = self._get_lexp(theta1 + i, theta2 + i, n_iterations)
            if not error:
                theta1_list.append(theta1[i])
                theta2_list.append(theta2[i])
                ratio_list.append(theta2[i] / theta1[i])
            else:
                computed = "t1=%f,t2=%f,ratio=%f" % (theta1[i], theta2[i], theta2[i]/theta1[i])
                print "Error: Computed values (%s) will be ignored" % computed

        m1 = numpy.average(theta1_list)
        m2 = numpy.average(theta2_list)
        m  = numpy.average(ratio_list)
        stddev1 = numpy.std(theta1_list)
        stddev2 = numpy.std(theta2_list)
        stddev  = numpy.std(ratio_list)

        if verbose:
            print "nb experiments: %d"%n_experiments
            print "nb experiments without error: %d"%len(theta1_list)
            print "nb iterations : %d"%n_iterations
            print "top L. exp    : %f   (%f)"%(m1, stddev1)
            print "second L. exp : %f   (%f)"%(m2, stddev2)
            print "ratio         : %f   (%f)"%(m, stddev)

        free(theta1)
        free(theta2)

        return [m1,m2]

    def invariant_measure_dict(self, int n_iterations, int ndivs, str norm='1'):
        r"""
        INPUT:

        - ``n_iterations`` - integer, number of iterations
        - ``ndvis`` - integer (less than or equal to 256), number of divisions per dimension
        - ``v`` - initial vector (default: ``None``)
        - ``norm`` -- string (default: ``'sup'``), either ``'sup'`` or
          ``'1'``, the norm used for the orbit of the algo
        - ``verbose`` -- bool (default: ``False``)

        OUTPUT:

        dict

        EXAMPLES

        ::

            sage: D = brun.invariant_measure_dict(4, 10, verbose=True)
            0.102209671626 0.241453007005 0.656337321369
            1 2 6
            0.134744020568 0.318309886184 0.546946093248
            1 3 5
            0.197661690901 0.335396102174 0.466942206924
            1 3 4
            0.197931587793 0.297412777066 0.504655635141
            1 2 5

        ::

            sage: D = brun.invariant_measure_dict(100000, 5)
            sage: D
            {(0, 1, 2): 21299, (0, 2, 2): 10079, (0, 0, 4): 19223, (1, 1, 1): 4912, (0, 1, 3): 19415, (0, 0, 3): 12307, (1, 1, 2): 12765}

        It is 1000 times faster using C counter instead of a python dict counter::

            sage: time D = brun.invariant_measure_dict(1000000, 10)
            Time: CPU 0.05 s, Wall: 0.05 s

        """
        cdef double s
        cdef int i,j
        cdef int X,Y

        # counter initialization
        # value for pos (i,j) is at C[i + ndivs*j]
        assert ndivs > 5, "ndivs should be at least 5"
        cdef int *C
        C = <int *> malloc((ndivs+1) * (ndivs+1) * sizeof(unsigned int))
        memset(C, 0, (ndivs+1) * (ndivs+1) * sizeof(unsigned int))

        cdef Point3d P = self._random_point()

        # Normalize (x,y,z)
        s = P.x + P.y + P.z
        P.x /= s; P.y /= s; P.z /= s

        if norm == '1':
            for i from 0 <= i < n_iterations:
                self._one_step(P)   # apply algo
                s = P.x + P.y + P.z # norm 1
                P.x /= s; P.y /= s; P.z /= s
                X = <int> (P.x * ndivs)
                Y = <int> (P.y * ndivs)
                assert X >= 0 and X <= ndivs
                assert Y >= 0 and Y <= ndivs
                C[X + (ndivs+1)*Y] += 1

        elif norm == 'sup':
            for i from 0 <= i < n_iterations:
                # Apply algo
                self._one_step(P)
                s = P.z # norm sup
                P.x /= s; P.y /= s; P.z = 1
                X = <unsigned int> (P.x * ndivs)   # in range(0,ndivs)
                Y = <unsigned int> (P.y * ndivs)   # in range(0,ndivs)
                assert X >= 0 and X <= ndivs
                assert Y >= 0 and Y <= ndivs
                C[X + (ndivs+1)*Y] += 1

        else:
            raise ValueError("norm must be '1' or 'sup'.")

        # Translate the counter into a python dict
        D = {}
        for j from 0 <= j <= ndivs:
            for i from 0 <= i <= j:
                c = C[i + ndivs*j]
                if c:
                    D[(i,j)] = c

        free(C)
        Point3d_free(&P)
        return D

    def plot_invariant_measure(self, n_iterations, ndivs, norm='sup'):
        r"""
        Return a matplotlib graph of the invariant measure.

        INPUT:

        - ``n_iterations`` - integer, number of iterations
        - ``ndvis`` - integer, number of divisions per dimension
        - ``norm`` -- string (default: ``'sup'``), either ``'sup'`` or
          ``'1'``, the norm used for the orbit points

        EXAMPLES::

            sage: algo.arrevert.plot_invariant_measure(1000000, 80) # not tested
            Creation du fichier mesure_arrevert_iter1000000_div80.png
            sage: algo.brun.plot_invariant_measure(1000000, 40, norm='sup')
            Creation du fichier mesure_brun_iter1000000_div40.png

        """
        D = self.invariant_measure_dict(n_iterations, ndivs, norm=norm)
        mx,my = map(max, zip(*D.keys()))
        len_D = float(len(D))
        the_mean = n_iterations / len_D

        X = [[i for i in range(mx+1)] for j in range(my+1)]
        Y = [[j for i in range(mx+1)] for j in range(my+1)]
        Z = [[D.get((i,j),0)/the_mean for i in range(mx+1)] for j in range(my+1)]

        title = "Algo=%s, nbiter=%s, ndivs=%s" % (self.name(), n_iterations, ndivs)
        filename = 'mesure_%s_iter%s_div%s.png' % (self.name(), n_iterations, ndivs)

        self._plot_wideframe(X,Y,Z,title,filename)

    def plot_invariant_measure_inverse(self, n_iterations, ndivs, norm='sup'):
        r"""
        Return a matplotlib graph of the inverse of the invariant measure.

        INPUT:

        - ``n_iterations`` - integer, number of iterations
        - ``ndvis`` - integer, number of divisions per dimension
        - ``norm`` -- string (default: ``'sup'``), either ``'sup'`` or
          ``'1'``, the norm used for the orbit points

        EXAMPLES::

            sage: algo.arrevert.plot_invariant_measure_inverse(1000000, 80) # not tested
            Creation du fichier mesure_arrevert_iter1000000_div80.png

        """
        D = self.invariant_measure_dict(n_iterations, ndivs, norm=norm)
        mx,my = map(max, zip(*D.keys()))
        len_D = float(len(D))
        the_mean = n_iterations / len_D

        E = dict((key,the_mean/value) for key,value in D.iteritems())

        X = [[i for i in range(mx+1)] for j in range(my+1)]
        Y = [[j for i in range(mx+1)] for j in range(my+1)]
        Z = [[E.get((i,j),0) for i in range(mx+1)] for j in range(my+1)]

        title = "Algo=%s, nbiter=%s, ndivs=%s" % (self.name(), n_iterations, ndivs)
        filename = 'mesure_inversed_%s_iter%s_div%s.png' % (self.name(), n_iterations, ndivs)

        self._plot_wideframe(X,Y,Z,title,filename)

    def _plot_wideframe(self, X,Y,Z, title, filename):
        r"""
        EXAMPLES::
        """
        from mpl_toolkits.mplot3d import axes3d
        import matplotlib.pyplot as plt
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ax.plot_wireframe(X, Y, Z, rstride=1, cstride=1)

        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')

        ax.set_title(title)
        plt.savefig(filename)
        print "Creation du fichier %s" % filename

