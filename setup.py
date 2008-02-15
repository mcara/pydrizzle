from distutils.core import setup, Extension
import sys, string, os.path, shutil
from distutils import sysconfig
from distutils.command.install_data import install_data

if not hasattr(sys, 'version_info') or sys.version_info < (2,3,0,'alpha',0):
    raise SystemExit, "Python 2.3 or later required to build pydrizzle."

try:
    import numpy
    import numpy.numarray as nn
except ImportError:
    "Numpy was not found. It may not be installed or it may not be on your PYTHONPATH. Pydrizzle requires numpy v 1.0.2 or later.\n"


if numpy.__version__ < "1.0.2":
    raise SystemExit, "Numpy 1.0.2 or later required to build pydrizzle."

print "Building C extensions using NUMPY."

numpyinc = numpy.get_include()
numpynumarrayinc = nn.get_numarray_include_dirs()

pythonlib = sysconfig.get_python_lib(plat_specific=1)
pythoninc = sysconfig.get_python_inc()
ver = sysconfig.get_python_version()
pythonver = 'python' + ver

f2c_inc_dir = []
f2c_lib_dir = []


if sys.platform != 'win32':
    pydrizzle_libraries = ['f2c', 'm']
    EXTRA_LINK_ARGS = []
else:
    pydrizzle_libraries = ['vcf2c']
    EXTRA_LINK_ARGS = ['/NODEFAULTLIB:MSVCRT']

args = sys.argv[:]

for a in args:
    if a.startswith('--local='):
        dir = os.path.abspath(a.split("=")[1])
        sys.argv.extend([
                "--install-lib="+dir,
                ])
        #remove --local from both sys.argv and args
        args.remove(a)
        sys.argv.remove(a)

class smart_install_data(install_data):
    def run(self):
        #need to change self.install_dir to the library dir
        install_cmd = self.get_finalized_command('install')
        self.install_dir = getattr(install_cmd, 'install_lib')
        return install_data.run(self)

def getF2CDirs(args):
    """ Defines the location of the F2C include and library directories. """
    if "--help" in sys.argv:
        print >>sys.stderr
        print >>sys.stderr, " options:"
        print >>sys.stderr, "--with-f2c=<f2c-dir> "
    for a in args:
        if string.find(a, '--with-f2c=') != -1:
            f2cdir = os.path.abspath(a.split('=')[1])
            sys.argv.remove(a)
            if os.path.exists(os.path.join(f2cdir, 'f2c.h')):
                f2c_inc_dir.append(f2cdir)
            elif os.path.exists(os.path.join(f2cdir, 'include','f2c.h')):
                f2c_inc_dir.append(os.path.join(f2cdir, 'include'))
            else:
                raise SystemExit, "f2c.h needed to build pydrizzle."
            if os.path.exists(os.path.join(f2cdir, 'libf2c.a')) or \
                   os.path.exists(os.path.join(f2cdir, 'libf2c.so')) or \
                   os.path.exists(os.path.join(f2cdir, 'vcf2c.lib')) :
                f2c_lib_dir.append(f2cdir)
            elif os.path.exists(os.path.join(f2cdir, 'lib','libf2c.a')) or \
                     os.path.exists(os.path.join(f2cdir, 'lib','libf2c.so')) or \
                     os.path.exists(os.path.join(f2cdir, 'lib','vcf2c.lib')) :
                f2c_lib_dir.append(os.path.join(f2cdir, 'lib'))
            else:
                raise SystemExit, "libf2c needed to build pydrizzle."


def getNumpyExtensions():
    ext = [Extension("pydrizzle.arrdriz",['src/arrdrizmodule.c',
                                'src/tdriz.c','src/tblot.c','src/twdriz.c',
                                'src/drutil.c','src/doblot.c','src/drcall.c',
                                'src/inter2d.c','src/bieval.c'],
                   define_macros=[('NUMPY', '1')],
                   include_dirs=[pythoninc] + [numpyinc]+ numpynumarrayinc+ f2c_inc_dir,
                   library_dirs=f2c_lib_dir,
                   extra_link_args=EXTRA_LINK_ARGS,
                   libraries=pydrizzle_libraries)]

    return ext


def dosetup(ext):
    r = setup(name = "pydrizzle",
              version = "6.0.0",
              description = "Geometrically correct and combine images using Drizzle",
              author = "Warren Hack",
              author_email = "help@stsci.edu",
              license = "http://www.stsci.edu/resources/software_hardware/pyraf/LICENSE",
              platforms = ["Linux","Solaris", "MacOS X", "Windows"],
              packages=['pydrizzle','pydrizzle/traits102', 'pydrizzle/distortion'],
              package_dir={'pydrizzle':'lib','pydrizzle/traits102':'traits102', 'pydrizzle/distortion':'lib/distortion'},
              cmdclass = {'install_data':smart_install_data},
              data_files = [('pydrizzle',['lib/LICENSE.txt'])],
              ext_modules=ext)
    return r



if __name__ == "__main__":
    getF2CDirs(args)
    ext = getNumpyExtensions()
    dosetup(ext)

