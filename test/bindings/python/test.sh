SCRIPTDIR=$(readlink -f "$0")
BASEDIR=$(dirname "$SCRIPTDIR")
BUILDDIR=${BASEDIR}/../../../build/
echo $BUILDDIR
cd $BUILDDIR
cd bindings/python
python3 setup.py build_ext --inplace
cd $BASEDIR
LD_LIBRARY_PATH=${BUILDDIR}/src:$LD_LIBRARY_PATH nosetests3 -s test.py
