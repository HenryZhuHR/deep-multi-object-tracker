if(BUILD_ONNXRUNTIME)
    set(EXECUTABLE_NAME mot-infer-test)
    include_directories(${detector_INCLUDE_DIRS})

    add_executable(${EXECUTABLE_NAME} mot-infer-test.cpp)
    # target_include_directories(${EXECUTABLE_NAME} PRIVATE
    #     ${detector_INCLUDE_DIRS})
    # link_directories(${detector_LIBRARY_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        detector
        ${GFLAGS_LIBRARIES}
        ${OpenCV_LIBS}
    )
    unset(EXECUTABLE_NAME)
endif()