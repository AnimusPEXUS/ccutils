wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    logger->Log(
        wayround_i2p::ccutils::logger::LoggerMSGType::Info,
        "setting iitm[\"x\"] to 123.."
    );

    iitm["x"] = 123;

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
