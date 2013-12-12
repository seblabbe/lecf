Lyapunov exponents for multidimensional continued fractions (LECF)
==================================================================

To compile just do

    make

And then you get a bunch of executable.

The relevant places to look at is the repository algorithms which
contain a bunch of algorithms and lecf.c which is a template to
generate a C file from the instructions given by an algorithm.

Each file in the repository algorithms/ describe the operation of a
2-dimensional continued fraction algorithms. If you want to create
your own MCF, just add a file in that repository and the Makefile
will take care of it!
