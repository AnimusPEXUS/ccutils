wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{
    opts.Log(
        wayround_i2p::ccutils::tst::Info,
        "setting iitm[\"x\"] to 123.."
    );

    opts.iitm["x"] = 123;

    return {
        true
    };
}

wayround_i2p::ccutils::tst::TSTInfo main_001_i = {
    .group_name        = "main",
    .test_name         = "001",
    .description_short = "sample test description",
    .description       = "long test description",
    .expected_failure  = true,
    .func              = main_001
};
