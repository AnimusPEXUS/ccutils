
#include <any>
#include <iostream>

#include <experimental/scope>

#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

#include "main_001.cpp"
#include "main_002.cpp"

#include "main_003_unicode.cpp"

int main(int argc, char **args)
{
    wayround_i2p::ccutils::tst::run_tests_Parameters params{
        .title       = "test suite for WayRound.I2P test suite tool",
        .description = "currently I use it while developping suite itself",
        .uri         = "https://github.com/AnimusPEXUS/ccutils/test_suite_tool"
    };

    params.AddTest(main_001_i);
    params.AddTest(main_002_i);
    params.AddTest(main_003_unicode_i);

    return wayround_i2p::ccutils::tst::run_tests(argc, args, params);
}
