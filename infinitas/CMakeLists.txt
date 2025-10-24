file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(infinitas ${SRC_FILES})

target_compile_features(infinitas PUBLIC cxx_std_23) 

target_link_libraries(infinitas virtual-machine compiler parser lexer shared)