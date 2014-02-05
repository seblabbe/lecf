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
    sage: rows = mcf.compare_algos_for_lyapunov(10000, 100, verbose=False)
    sage: table(rows=rows)
      Algorithm                       #Exp   Theta1 (std)        Theta2 (std)         1-Theta2/Theta1
      Arnoux-Rauzy Poincare           100    0.44418 (0.01129)   -0.17249 (0.00554)   1.38834
      Baladi-Nogueira                 100    2.32352 (0.01126)   -0.71746 (0.00557)   1.30878
      Baladi-Nogueira Algo A          100    2.32218 (0.00904)   -0.71718 (0.00653)   1.30883
      Baladi-Nogueira Algo B          100    1.28712 (0.00462)   -0.48268 (0.00381)   1.37501
      Baladi-Nogueira modified        100    2.50955 (0.01175)   -0.85272 (0.00696)   1.33978
      (multiplicative floor) Brun     100    0.67044 (0.00416)   -0.24705 (0.00336)   1.36848
      (multiplicative nearest) Brun   100    0.85912 (0.00518)   -0.33466 (0.00368)   1.38954
      Jacobi-Perron                   100    1.20151 (0.00939)   -0.44855 (0.00418)   1.37332
      (multiplicative floor) Selmer   100    0.18217 (0.01019)   -0.07077 (0.00423)   1.38848

::

    sage: %time rows = mcf.compare_algos_for_lyapunov(100000, 100, verbose=False)
    CPU times: user 9.76 s, sys: 0.03 s, total: 9.78 s
    Wall time: 9.87 s
    sage: table(rows=rows)
      Algorithm                       #Exp   Theta1 (std)        Theta2 (std)         1-Theta2/Theta1
      Arnoux-Rauzy Poincare           100    0.44395 (0.00471)   -0.17266 (0.00218)   1.38891
      Baladi-Nogueira                 100    2.32329 (0.00331)   -0.71723 (0.00179)   1.30871
      Baladi-Nogueira Algo A          100    2.32295 (0.00344)   -0.71757 (0.00186)   1.30890
      Baladi-Nogueira Algo B          100    1.28664 (0.00154)   -0.48245 (0.00129)   1.37497
      Baladi-Nogueira modified        99     2.51149 (0.00395)   -0.85325 (0.00210)   1.33974
      (multiplicative floor) Brun     100    0.66954 (0.00131)   -0.24663 (0.00097)   1.36836
      (multiplicative nearest) Brun   100    0.85861 (0.00156)   -0.33389 (0.00123)   1.38887
      Jacobi-Perron                   100    1.20109 (0.00270)   -0.44869 (0.00128)   1.37356
      (multiplicative floor) Selmer   100    0.18315 (0.00276)   -0.07089 (0.00118)   1.38706

::

    sage: %time rows = mcf.compare_algos_for_lyapunov(1000000, 100, verbose=False)
    CPU times: user 92.47 s, sys: 0.12 s, total: 92.60 s
    Wall time: 92.79 s
    sage: table(rows=rows, header_row=True)
      Algorithm                       #Exp   Theta1 (std)         Theta2 (std)          1-Theta2/Theta1
    +-------------------------------+------+--------------------+---------------------+-----------------+
      Arnoux-Rauzy Poincare           100    0.44301 (0.00147)   -0.17224 (0.00067)   1.38879
      Baladi-Nogueira                 86     2.32316 (0.00098)   -0.71729 (0.00051)   1.30875
      Baladi-Nogueira Algo A          82     2.32296 (0.00099)   -0.71727 (0.00052)   1.30877
      Baladi-Nogueira Algo B          100    1.28671 (0.00043)   -0.48249 (0.00037)   1.37498
      Baladi-Nogueira modified        93     2.51134 (0.00117)   -0.85338 (0.00064)   1.33980
      (multiplicative floor) Brun     100    0.66948 (0.00042)   -0.24661 (0.00030)   1.36835
      (multiplicative nearest) Brun   100    0.85867 (0.00053)   -0.33387 (0.00034)   1.38882
      Jacobi-Perron                   100    1.20041 (0.00088)   -0.44849 (0.00041)   1.37361
      (multiplicative floor) Selmer   100    0.18264 (0.00104)   -0.07070 (0.00044)   1.38708

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
