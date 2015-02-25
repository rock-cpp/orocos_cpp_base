rock_library(orocos_cpp_base
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(orocos_cpp_base_bin Main.cpp
    DEPS orocos_cpp_base)

