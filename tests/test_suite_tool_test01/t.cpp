
#include <iostream>

#include <experimental/scope>

#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

wayround_i2p::ccutils::tst::TSTFuncResult test_001(
    wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{
    return {
        true
    };
}

wayround_i2p::ccutils::tst::TSTInfo test_001i = {
    .group_name        = "main",
    .test_name         = "test_001i",
    .description_short = "sample test description",
    .description       = "long test description",
};

int main(int argc, char **args)
{
    wayround_i2p::ccutils::tst::run_tests_Parameters params;

    params.AddTest(test_001i);

    return wayround_i2p::ccutils::tst::run_tests(params);
}
