file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(shared STATIC ${SRC_FILES})

target_link_libraries(shared PUBLIC spdlog::spdlog boost_multiprecision PkgConfig::GMPXX)

target_compile_features(shared PUBLIC cxx_std_23)

target_include_directories(shared PUBLIC include)