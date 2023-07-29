cmake_minimum_required(VERSION 3.5)

project(DeepMOT LANGUAGES CXX)

option(BUILD_ONNXRUNTIME "BUILD ONNXRUNTIME" ON) # 默认开启编译 ONNXRUNTIME

message(STATUS "[INFO] BUILD_ONNXRUNTIME: ${BUILD_ONNXRUNTIME}")
message(STATUS "[INFO] DEBUG: ${CMAKE_BUILD_TYPE}")

# if(${CMAKE_BUILD_TYPE}==Debug)
#     add_definitions(-DDEBUG)
# endif()
add_definitions(-DDEBUG)

string(ASCII 27 Esc)
set(ColourReset "${Esc}[m")
set(BoldRed "${Esc}[1;31m")
set(BoldGreen "${Esc}[1;32m")
set(BoldYellow "${Esc}[1;33m")
set(BoldMagenta "${Esc}[1;35m")
set(BoldCyan "${Esc}[1;36m")
set(BoldWhite "${Esc}[1;37m")

# CMAKE_C_COMPILER
if(NOT CMAKE_C_COMPILER)
    if(APPLE)
        set(CMAKE_C_COMPILER /usr/bin/clang)
    elseif(UNIX AND NOT APPLE)
        set(CMAKE_C_COMPILER /usr/bin/gcc)
    elseif(WIN32)
        set(CMAKE_C_COMPILER C:/mingw64/bin/x86_64-w64-mingw32-gcc.exe)
    else()
        message(FATAL_ERROR "Unsupported platform, please set CMAKE_C_COMPILER")
    endif()
endif()

message(STATUS "[INFO] C compiler: ${CMAKE_C_COMPILER}")

# CMAKE_CXX_COMPILER
if(NOT CMAKE_CXX_COMPILER)
    if(APPLE)
        set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    elseif(UNIX AND NOT APPLE)
        set(CMAKE_CXX_COMPILER /usr/bin/g++)
    elseif(WIN32)
        set(CMAKE_CXX_COMPILER C:/mingw64/bin/x86_64-w64-mingw32-g++.exe)
    else()
        message(FATAL_ERROR "Unsupported platform, please set CMAKE_CXX_COMPILER")
    endif()
endif()

message(STATUS "[INFO] CXX compiler: ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------
# [dependencies] gflags
# ------------------------------------------
find_package(gflags REQUIRED COMPONENTS static)
include_directories(${GFLAGS_INCLUDE_DIR})
link_directories(${gflags_LIBS})
message(STATUS "[INFO] gflags status:")
message(STATUS "       include  : ${GFLAGS_INCLUDE_DIR}")
message(STATUS "       lib      : ${GFLAGS_LIBRARIES}")

# ------------------------------------------
# [dependencies] opencv
# ------------------------------------------
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS}/include)
link_directories(${OpenCV_LIBS})
message(STATUS "[INFO] OpenCV status:")
message(STATUS "       version  : ${OpenCV_VERSION}")
message(STATUS "       include  : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "       libs     : ${OpenCV_LIBS}")

# ------------------------------------------
# set output path
# ------------------------------------------
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/bin)


include_directories(include)
aux_source_directory(src DIR_SRCS)

set(EXECUTABLE_NAME mot-infer-test)
add_executable(${EXECUTABLE_NAME} 
    app/mot-infer-test.cpp
    ${DIR_SRCS}
)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    ${GFLAGS_LIBRARIES}
    ${OpenCV_LIBS}
)
unset(EXECUTABLE_NAME)


if(NOT EXISTS ${CMAKE_INSTALL_PREFIX}/resource)
    install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/resource
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
endif()