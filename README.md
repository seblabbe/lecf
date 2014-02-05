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
mcf.so). You can launch Python, IPython or Sage inside the directory and try::

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

You can check that your algorithm exists with::

    >>> import mcf
    >>> mcf.algorithm_names()
    ['arp',
     'baladi_nogueira',
     'baladi_nogueira_A',
     'baladi_nogueira_B',
     'baladi_nogueira_B_mod',
     'baladi_nogueira_mod',
     'brun_mf',
     'brun_mn',
     'jacobi_perron',
     'selmer']

And check that it works with::

    >>> mcf.check_all()
    check arp... done
    check baladi_nogueira... done
    check baladi_nogueira_A... done
    check baladi_nogueira_B... done
    check baladi_nogueira_B_mod... done
    check baladi_nogueira_mod... done
    check brun_mf... done
    check brun_mn... done
    check jacobi_perron... done
    check selmer... done

Make some comparison::

    sage: import mcf
    sage: %time rows = mcf.compare_algos_for_lyapunov(1000000, 100, verbose=False)
    CPU times: user 92.47 s, sys: 0.12 s, total: 92.60 s
    Wall time: 92.79 s
    sage: table(rows=rows)
      Algorithm                       #Exp   Theta1 (std)        Theta2 (std)         1-Theta2/Theta1
      Arnoux-Rauzy Poincare           100    0.44279 (0.00176)   -0.17216 (0.00076)   1.38880
      Baladi-Nogueira                 84     2.32307 (0.00094)   -0.71726 (0.00047)   1.30875
      Baladi-Nogueira Algo A          81     2.32315 (0.00087)   -0.71721 (0.00054)   1.30872
      Baladi-Nogueira Algo B          100    1.28671 (0.00045)   -0.48248 (0.00038)   1.37497
      Baladi-Nogueira modified        91     2.51147 (0.00109)   -0.85344 (0.00061)   1.33982
      (multiplicative floor) Brun     100    0.66950 (0.00042)   -0.24659 (0.00031)   1.36831
      (multiplicative nearest) Brun   100    0.85861 (0.00050)   -0.33388 (0.00036)   1.38887
      Jacobi-Perron                   100    1.20041 (0.00089)   -0.44842 (0.00043)   1.37355
      (multiplicative floor) Selmer   100    0.18266 (0.00130)   -0.07075 (0.00054)   1.38732

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
