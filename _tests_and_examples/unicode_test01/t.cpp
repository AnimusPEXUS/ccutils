

#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

#include "main_tests.cpp"

int main(int argc, char **args)
{
    wayround_i2p::ccutils::tst::run_tests_Parameters params{
        .title = "WayRound.I2P ccutils::unicode test suite",
        .uri   = "https://github.com/AnimusPEXUS/ccutils"
    };

    params.AddTest(main_001_i);


    return wayround_i2p::ccutils::tst::run_tests(argc, args, params);
}
