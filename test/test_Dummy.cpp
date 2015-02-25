#include <boost/test/unit_test.hpp>
#include <orocos_cpp_base/Dummy.hpp>

using namespace orocos_cpp_base;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    orocos_cpp_base::DummyClass dummy;
    dummy.welcome();
}
