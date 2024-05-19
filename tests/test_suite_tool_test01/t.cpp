
#include <iostream>

#include <experimental/scope>

#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

int main(int argc, char **args)
{
    wayround_i2p::ccutils::tst::run_tests_Parameters params;

    wayround_i2p::ccutils::tst::run_tests(params);

    return 0;
}
