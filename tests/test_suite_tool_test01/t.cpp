
#include <any>
#include <iostream>

#include <experimental/scope>

#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

wayround_i2p::ccutils::tst::TSTFuncResult test_001(
    wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{
    opts.Log(
        wayround_i2p::ccutils::tst::Info,
        "setting iim[\"x\"] to 123.."
    );

    opts.ingroup_inter_test_memory["x"] = 123;

    return {
        true
    };
}

wayround_i2p::ccutils::tst::TSTInfo test_001i = {
    .group_name        = "main",
    .test_name         = "test_001i",
    .description_short = "sample test description",
    .description       = "long test description",
    .expected_failure  = true,
    .func              = test_001
};

wayround_i2p::ccutils::tst::TSTFuncResult test_002(
    wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{
    opts.Log(
        wayround_i2p::ccutils::tst::Info,
        std::format(
            "getting iim[\"x\"]..: {}",
            opts.ingroup_inter_test_memory["x"].type().name()
        )
    );

    return {
        true
    };
}

wayround_i2p::ccutils::tst::TSTInfo test_002i = {
    .group_name        = "main",
    .test_name         = "test_002i",
    .description_short = "sample test description",
    .description       = "long test description",
    .expected_failure  = false,
    .func              = test_002
};

int main(int argc, char **args)
{
    wayround_i2p::ccutils::tst::run_tests_Parameters params{
        .title       = "test suite for WayRound.I2P test suite tool",
        .description = "currently I use it while developping suite itself",
        .uri         = "https://github.com/AnimusPEXUS/ccutils/test_suite_tool"
    };

    params.AddTest(test_001i);
    params.AddTest(test_002i);

    return wayround_i2p::ccutils::tst::run_tests(params);
}
