cmake_minimum_required(VERSION 3.28)
project(test_install_lbp)

set(CMAKE_CXX_STANDARD 17)  # or newer
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
# set(CMAKE_PREFIX_PATH /path/to/halide/directory)

find_package(Halide REQUIRED)

add_executable(gray_lbp_gen src/main.cpp)
target_link_libraries(gray_lbp_gen PRIVATE Halide::Halide Halide::Tools Halide::ImageIO)