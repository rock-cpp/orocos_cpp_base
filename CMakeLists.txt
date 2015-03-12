# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
rock_init(orocos_cpp_base 0.1)
set(OROCOS_TARGET "gnulinux")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x" )
rock_standard_layout()
