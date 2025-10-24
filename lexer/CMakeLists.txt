file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(lexer STATIC ${SRC_FILES})

target_include_directories(lexer PUBLIC include)

target_compile_features(lexer PUBLIC cxx_std_23)

target_link_libraries(lexer PRIVATE shared)