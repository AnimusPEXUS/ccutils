wayround_i2p::ccutils::tst::TSTFuncResult main_002(
    const wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{
    opts.Log(
        wayround_i2p::ccutils::tst::Info,
        std::format(
            "getting iitm[\"x\"]..: {}",
            std::any_cast<int>(opts.iitm["x"])
        )
    );

    return {
        true
    };
}

wayround_i2p::ccutils::tst::TSTInfo main_002_i = {
    .group_name        = "main",
    .test_name         = "002",
    .description_short = "sample test description",
    .description       = "long test description",
    .expected_failure  = false,
    .func              = main_002
};
