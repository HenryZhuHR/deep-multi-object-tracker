
proj_home=$(pwd)

DEFAULT=$(echo -en '\033[0m')
LRED=$(echo -en '\033[01;31m')
LGREEN=$(echo -en '\033[01;32m')
LYELLOW=$(echo -en '\033[01;33m')
LBLUE=$(echo -en '\033[01;34m')
LCYAN=$(echo -en '\033[01;36m')

if [ "$(uname)" = "Darwin" ]; then
    NUM_CORES=$(sysctl -n hw.ncpu)
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
    NUM_CORES=$(nproc --all)
else
    NUM_CORES=4
fi

source_dir=$proj_home/temp
install_dir=$proj_home/libs


file_dir=$(cd $(dirname $0); pwd)
# TODO 修改源码包的版本号
source $file_dir/compile-eigen.sh "3.4.0" $source_dir $install_dir
source $file_dir/compile-gflags.sh "2.2.2" $source_dir $install_dir
export ONNXRUNTIME_HOME=$proj_home/libs/onnxruntime
source $file_dir/compile-opencv.sh "4.8.0" $source_dir $install_dir

mkdir $proj_home/build
cd $proj_home/build
rm CMakeCache.txt
cmake .. \
    -DCMAKE_INSTALL_PREFIX=$proj_home/install \
    -DCMAKE_BUILD_TYPE:STRING=Debug
make -j${NUM_CORES}
make install


echo ""
echo "${LCYAN} RUN: ${DEFAULT}"
echo ""
cd $proj_home/install/bin
./mot-infer-test