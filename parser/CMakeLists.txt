file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(parser STATIC ${SRC_FILES})

target_include_directories(parser PUBLIC include)
target_include_directories(parser PRIVATE details)

target_compile_features(parser PUBLIC cxx_std_23) 

target_link_libraries(parser PRIVATE compiler shared)
