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

    sage: %time rows = mcf.compare_algos_for_lyapunov(1000000, 100)
    CPU times: user 106.68 s, sys: 0.21 s, total: 106.89 s
    Wall time: 107.08 s
    sage: table(rows=rows)
      Algorithm                  #Exp   Theta1 (std)        Theta2 (std)         1-Theta2/Theta1
      Brun (multi nearest)       100    0.85859 (0.00054)   -0.33390 (0.00040)   1.38890 (0.00468)
      ARP (multi)                100    0.82376 (0.00040)   -0.32031 (0.00028)   1.38884 (0.00347)
      Arnoux-Rauzy Poincare      100    0.44267 (0.00179)   -0.17209 (0.00079)   1.38876 (0.02216)
      Selmer (multi)             100    0.18270 (0.00102)   -0.07071 (0.00045)   1.38704 (0.03075)
      Baladi-Nogueira Algo B     100    1.28675 (0.00044)   -0.48254 (0.00037)   1.37500 (0.00296)
      Jacobi-Perron              100    1.20044 (0.00093)   -0.44841 (0.00042)   1.37354 (0.00459)
      Brun (multi)               100    0.66941 (0.00040)   -0.24654 (0.00030)   1.36829 (0.00491)
      ARP (multi nearest)        100    0.89755 (0.00046)   -0.32314 (0.00031)   1.36002 (0.00409)
      Baladi-Nogueira modified   89     2.51148 (0.00135)   -0.85339 (0.00062)   1.33980 (0.00371)
      Baladi-Nogueira Algo A     79     2.32297 (0.00105)   -0.71734 (0.00057)   1.30880 (0.00402)
      Baladi-Nogueira            87     2.32303 (0.00110)   -0.71719 (0.00053)   1.30873 (0.00391)

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
