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

def algorithm_names():
    cdef char ** algos = list_of_algorithms()
    l = []
    for i in xrange(number_of_algorithms()):
        l.append(str(algos[i]))
    return l

# clean
cdef class MCFAlgorithm(object):
    def __init__(self, name=None):

        assert isinstance(name, str)
        cdef cfa c = get_cfa_from_name(name)

        if c is NULL:
            raise ValueError("no algorithm named %s. Must be one of: %s"%(name, ', '.join(algorithm_names())))

        self._short_name = name
        self._description = c.description
        self._get_lexp = c.get_lexp
        self._one_step = c.one_step

        free(c)

    def name(self):
        return self._short_name

    def long_name(self):
        return self._description

    def specifications(self):
        r"""
        Return the domain and the specification of the algorithm.
        """
        raise NotImplementedError

    def __repr__(self):
        return self.long_name()

    def _check(self):
        r"""
        Do some check to test consistency of the algorithm.
        """
        cdef Point3d P = Point3d_new()
        cdef double s
        cdef int i

        Point3d_set_random(P)

        s = P.x * P.u + P.y * P.v + P.z * P.w
        if s < -0.000001 or s > 0.000001:
            raise AssertionError("scalar product is not 0 after Point3d_set_random")

        for i in range(100):
            self._one_step(P)
            s = P.x * P.u + P.y * P.v + P.z * P.w
            if s < -0.000001 or s > 0.000001:
                raise AssertionError("orthogonality not preserved by the algo")

        Point3d_free(&P)

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

        assert n_experiments > 1
        assert n_iterations > 100

        theta1 = <double *> malloc(n_experiments * sizeof(double))
        theta2 = <double *> malloc(n_experiments * sizeof(double))

        m1 = m2 = 0
        for i in range(n_experiments):
            self._get_lexp(theta1 + i, theta2 + i, n_iterations)
            m1 += theta1[i]
            m2 += theta2[i]
            m += theta2[i] / theta1[i]

        m1 /= n_experiments
        m2 /= n_experiments
        m /= n_experiments

        if verbose:
            stddev1 = stddev2 = stddev = 0
            for i in range(n_experiments):
                stddev1 += (theta1[i] - m1) * (theta1[i] - m1)
                stddev2 += (theta2[i] - m2) * (theta2[i] - m2)
                stddev += (theta2[i] / theta1[i] - m) * (theta2[i]/theta1[i] - m)
            print "nb experiments: %d"%n_experiments
            print "nb iterations : %d"%n_iterations
            print "top L. exp    : %f   (%f)"%(m1, stddev1)
            print "second L. exp : %f   (%f)"%(m2, stddev2)
            print "ratio         : %f   (%f)"%(m, stddev)

        free(theta1)
        free(theta2)

        return [m1,m2]

    def invariant_measure_dict(self, int n_iterations, int ndivs, v=None,
            str norm='1', verbose=False):
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

        cdef Point3d P = Point3d_new()
        Point3d_set_random(P)
        Point3d_sort(P)        # order (x,y,z) such that x < y < z

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

