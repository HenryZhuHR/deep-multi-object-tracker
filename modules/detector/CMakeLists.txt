


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src  detector_src_list)
message(STATUS "[INFO] compile by:  ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "[INFO]              ${detector_src_list}")

set(detector_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE STRING INTERNAL )

add_library(detector SHARED ${detector_src_list})   # dynamic link library
target_include_directories(detector PUBLIC ${detector_INCLUDE_DIRS})
target_link_libraries(detector
    PUBLIC
    ${ONNXRUNTIME_LIBS}
)
