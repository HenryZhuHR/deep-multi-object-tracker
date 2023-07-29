# Deep Multi-Object Tracker


## 环境
Ubuntu 

## 安装

需要下载如下源码包，并放置在 `temp` 目录下，首次运行的时候会自动编译这些库，编译后安装目录为 `libs`
- [`eigen-3.4.0.tar.gz`](https://eigen.tuxfamily.org/index.php?title=Main_Page)(`.tar.bz2` 也可以)
- [`gflags-2.2.2.tar.gz`](https://github.com/gflags/gflags/tree/v2.2.2)
- [`onnxruntime-linux-x64-gpu-1.15.1.tgz`](https://github.com/microsoft/onnxruntime/releases)
- [`opencv-4.5.5.tar.gz`](https://github.com/opencv/opencv/releases) 和 [`opencv_contrib-4.5.5.tar.gz`](https://github.com/opencv/opencv_contrib/releases/tag)
> 如果下载其他版本，则修改修改脚本`scripts/build.sh`中的版本号 ( 搜索 `# TODO 修改源码包的版本号`)

如果系统内已经安装了这些库，可以设置 `CMAKE_PREFIX_PATH` 和 `ONNXRUNTIME_HOME` 环境变量，
```shell
# 修改为 eigen 实际路径
export EIGEN_HOME=libs/eigen
# 修改为 gflags 实际路径
export GFLAGS_HOME=libs/gflags
# 修改为 opencv 实际路径
export OPENCV_HOME=libs/opencv
# 修改为 onnxruntime 实际路径
export ONNXRUNTIME_HOME=libs/onnxruntime-linux-x64-gpu-1.15.1
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$EIGEN_HOME/eigen/share/eigen3/cmake:$GFLAGS_HOME/gflags/lib/cmake/gflags:$OPENCV_HOME/lib/cmake/opencv4
```