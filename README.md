LECF
====

Why Lyapunov exponents for multidimensional continued fractions (LECF) ?
------------------------------------------------------------------------

An important problem of both theoretical and computational mathematics is the
ability to approximate the coordinates of a vector (v1, v2, v3) by rational
coordinates with the same denominator (m1/N, m2/N, m3/N). One attempt to do
this is through multidimensional continued fractions. The quality of
diophantine approximation is measured by the second (normalized) Lyapunov
exponents of the algorithm which is a real number between -1/2 and 1. If this
number is negative, then the algorithm is *strongly convergent* and it would be
*optimal* if it was *-1/2*.  Up to now, there is no optimal algorithm... we are
still looking for... but there might be none!

Compilation
-----------

To compile just do

    make

After a lot of compilation you get a running Python module (whose name is
mcf.so). You can launch Python, IPython or Sage inside the directory and try:

    >>> import mcf
    >>> B = mcf.MCFAlgorithm('brun_mf')
    >>> B
    (multiplicative floor) Brun
    >>> lexp = B.lyapunov_exponents()
    nb experiments: 100
    nb iterations : 10000
    top L. exp    : 0.669311   (0.001818)
    second L. exp : -0.246783   (0.000977)
    ratio         : -0.368712   (0.001664)
    >>> mcf.algorithm_names()
    ['baladi_nogueira', 'baladi_nogueira_mod', 'brun_mf', 'brun_mn', 'jacobi_perron', 'selmer']

The relevant place to look for the code is the directory src/. The instructions
for each algorithm are contained in src/algorithms/. At building time (ie when
make is run) the list of algorithms is generated on the fly. So if you feel,
you can implement your own algorithm by simply writing a new file in
src/algorithms/ and then run make.

You can check that your algorithm exists with

    >>> import mcf
    >>> mcf.algorithm_names()
    ['brun_mf', brun_mn', 'jacobi_perron', 'selmer']

And check that it works with

    >>> mcf.check_all()
    check brun_mf... done
    check brun_mn... done
    check jacobi_perron... done
    check selmer... done

Details of implementation
-------------------------

Some files are automatically generated from the list of algorithms contained in
src/algorithms/. Namely src/algorithms\_array.c, src/mcf\_algorithms.h and
src/mcf\_algorithms.c. The C template used to generate src/mcf\_algorithms.c is
src/lecf.c and the (python) script to generate the two others is
scripts/make\_mains\_include\_file. You can find the relevant lines in the
Makefile.

Help
----

To be able to compile you need gcc (the GNU C compiler), make and Python
headers. If you start modifying src/mcf.pyx or src/mcf.pxd then you need
Cython.

If you have troubles, feel free to e-mail me at vincent.delecroix@labri.fr
