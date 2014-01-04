from distutils.core import setup
from distutils.extension import Extension

try:
    import Cython
    print "Cython version %s found"%Cython.__version__
    from Cython.Build import cythonize
    USE_CYTHON = True
except ImportError:
    print "Warning: Cython version absent or inadapted. We do not cythonize mcf.pyx."
    USE_CYTHON = False

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
    extensions = cythonize(extensions)

setup(
    name        = "mcf",
    version     = "0.alpha1",
    author      = "V. Delecroix and S. Labbe",
    description = "Multidimensional Continued Fractions",
    ext_modules = extensions)
