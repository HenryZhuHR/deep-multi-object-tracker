version=$1
source_dir=$2
install_dir=$3

package_name="eigen"

if  [ -d "$install_dir/$package_name/share/eigen3/cmake" ]; then
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

package_dir="${source_dir}/${source_package_name}"
if [ ! -d $package_dir ];then
    if [ -f $package_dir.tar.gz ]; then
        tar xf $package_dir.tar.gz -C $source_dir
    elif [ -f $package_dir.tar.bz2 ]; then
        tar xf $package_dir.tar.bz2 -C $source_dir
    else
        echo "${LRED} $package_dir.tar.gz/bz2 not exist, exit.${DEFAULT}"
        exit 1
    fi
fi


if [ ! -d "$package_dir" ]; then
    echo "${LRED}package_dir ${package_dir} not exist, exit.${DEFAULT}"
    exit 1
fi

# start
# 如果存在变量 NUM_CORES
if [ ! -n "$NUM_CORES" ]; then
    NUM_CORES=1
fi
cd $package_dir
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$install_dir/$package_name
make -j${NUM_CORES}
make install

export EIGEN_HOME=$install_dir/$package_name
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$EIGEN_HOME/share/eigen3/cmake
