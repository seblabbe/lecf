Lyapunov exponents for multidimensional continued fractions (LECF)
==================================================================

To compile just do

    make

After a lot of compilation you get a running Python module (whose name
is mcf.so). You can launch Python, IPython or Sage inside the directory
and try:

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

The relevant place to look for the code is the directory src/. The
instructions for each algorithm are contained in src/algorithms/. At
building time (ie when make is run) the list of algorithms is generated
on the fly. So if you feel, you can implement your own algorithm by
simply writing a new file in src/algorithms/ and then run make.

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

