file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(virtual-machine STATIC ${SRC_FILES})

target_include_directories(virtual-machine PUBLIC include details)

target_compile_features(virtual-machine PUBLIC cxx_std_23)

target_link_libraries(virtual-machine PRIVATE shared)