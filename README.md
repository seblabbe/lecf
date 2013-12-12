Lyapunov exponents for multidimensional continued fractions (LECF)
==================================================================

To compile just do

    make

You get a lot of warning, but hopefuly you should obtain a running version
of the library. Launch Python, IPython or Sage and try

    >>> import mpc
    >>> B = mcf.MCFAlgorithm('brun_mf')
    >>> B
    (multiplicative floor) Brun
    >>> lexp = B.lyapunov_exponents()
    nb experiments: 100
    nb iterations : 10000
    top L. exp    : 0.669311   (0.001818)
    second L. exp : -0.246783   (0.000977)
    ratio         : -0.368712   (0.001664)

The relevant places to look for the code is in src. The instructions for
each algorithm are contained in src/algorithms. At building time (ie doing
make) the list of algorithms is generated on the fly. So if you feel in the
mood you can implement your own algorithm by simply putting in new file there
and then run make.
