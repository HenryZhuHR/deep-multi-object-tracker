version=$1
source_dir=$2
install_dir=$3

package_name="opencv"

if  [ -d "$install_dir/$package_name/lib/cmake/opencv4" ]; then
    echo "${LGREEN}-- [$package_name] has been installed, skip.${DEFAULT}"
    return 
fi


echo "${LCYAN}-- Start build $package_name${DEFAULT} "

if [ -n "$source_dir" ]; then
    echo "${LCYAN}   source_dir :${DEFAULT} $source_dir"
else
    echo "${LRED}source_dir is empty, exit.${DEFAULT}"
    exit 1
fi

if [ -n "$install_dir" ]; then
    echo "${LCYAN}   install_dir:${DEFAULT} $install_dir"
else
    echo "${LRED}install_dir is empty, exit.${DEFAULT}"
    exit 1
fi

if [ -n "$version" ]; then
    echo "${LCYAN}   version    :${DEFAULT} $version"
    source_package_name="${package_name}-${version}"
else
    echo "${LRED}version is empty, exit.${DEFAULT}"
    source_package_name="${package_name}"
fi
if [ -n "$version" ]; then
    source_contrib_package_name="${package_name}_contrib-${version}"
else
    source_contrib_package_name="${package_name}_contrib"
fi

package_dir="${source_dir}/${source_package_name}"
package_contrib_dir="${source_dir}/${source_contrib_package_name}"

if [ ! -d $package_dir ];then
    if [ -f $package_dir.tar.gz ]; then
        tar xf $package_dir.tar.gz -C $source_dir
    elif [ -f $package_dir.tar.bz2 ]; then
        tar xf $package_dir.tar.bz2 -C $source_dir
    else
        echo "${LRED} $package_dir.tar.gz not exist, exit.${DEFAULT}"
        exit 1
    fi
fi
if [ ! -d $package_contrib_dir ];then
    if [ -f $package_contrib_dir.tar.gz ]; then
        tar xf $package_contrib_dir.tar.gz -C $source_dir
    elif [ -f $package_contrib_dir.tar.bz2 ]; then
        tar xf $package_contrib_dir.tar.bz2 -C $source_dir
    else
        echo "${LRED} $package_contrib_dir.tar.gz not exist, exit.${DEFAULT}"
        exit 1
    fi
fi


if [ ! -d "$package_dir" ]; then
    echo "${LRED} ${package_dir} not exist, exit.${DEFAULT}"
    exit 1
fi
if [ ! -d "$package_contrib_dir" ]; then
    echo "${LRED}${package_contrib_dir} not exist, exit.${DEFAULT}"
    exit 1
fi

echo "路径 ${package_dir}"
echo "路径 ${package_contrib_dir}"

# start
# 如果存在变量 NUM_CORES
if [ ! -n "$NUM_CORES" ]; then
    NUM_CORES=1
fi

cd $package_dir
mkdir build
cd build
rm CMakeCache.txt
export PYTHON3_EXECUTABLE=$(which python3)
cmake .. \
    -DCMAKE_INSTALL_PREFIX=$install_dir/$package_name \
    -DOPENCV_EXTRA_MODULES_PATH=$package_contrib_dir/modules \
    -DBUILD_opencv_python2=OFF -DBUILD_opencv_python3=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DOPENCV_GENERATE_PKGCONFIG=ON \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_STATIC_LIBS=ON \
    -DBUILD_opencv_apps=ON \
    -DBUILD_EXAMPLES=OFF \
    -DWITH_ONNX=ON -DONNXRT_ROOT_DIR=$ONNXRUNTIME_HOME \
    -DOPENCV_DOWNLOAD_MIRROR_ID=gitee
    
make -j${NUM_CORES}
make install

export OPENCV_HOME=$install_dir/$package_name
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$OPENCV_HOME/lib/cmake/opencv4
