rock_library(orocos_cpp_base
        ProxyPort.cpp
        OrocosHelpers.cpp
    HEADERS 
        ProxyPort.hpp
        OrocosHelpers.hpp
    DEPS_PKGCONFIG
        orocos-rtt-${OROCOS_TARGET}
    DEPS_PLAIN 
        Boost_SYSTEM Boost_FILESYSTEM Boost_THREAD
)



