from distutils.core import setup
from distutils.extension import Extension

try:
    import Cython
    USE_CYTHON = True
    print "using cython"
except ImportError:
    USE_CYTHON = False
    print "cython is not present, use the default C-file"

extension = ".pyx" if USE_CYTHON else ".c"

sourcefiles = [
# cython files
"src/mcf" + extension,
# pure C files
"src/random.c",
"src/point3d.c",
"src/mcf_algorithms.c",
"src/mcf_template.c"
]

extensions = [Extension("mcf", sourcefiles)]

if USE_CYTHON:
    from Cython.Build import cythonize
    extensions = cythonize(extensions)

setup(
    name        = "mcf",
    version     = "0.alpha1",
    author      = "V. Delecroix and S. Labbe",
    description = "Multidimensional Continued Fractions",
    ext_modules = extensions)
