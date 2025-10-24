file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(compiler STATIC ${SRC_FILES})

target_link_libraries(compiler PUBLIC parser shared)

target_compile_features(compiler PUBLIC cxx_std_23)

target_include_directories(compiler PUBLIC include)