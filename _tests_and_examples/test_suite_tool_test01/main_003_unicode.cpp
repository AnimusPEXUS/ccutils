wayround_i2p::ccutils::tst::TSTFuncResult main_003_unicode(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{

    auto x = wayround_i2p::ccutils::unicode::UString("тестовая строка");

    logger->Log(
        wayround_i2p::ccutils::logger::LoggerMSGType::Info,
        std::format(
            "checking unicode. length of \"{}\" == {}. should be 15",
            x,
            x.length()
        )
    );

    return {
        x.length() == 15
    };
}

wayround_i2p::ccutils::tst::TSTInfo main_003_unicode_i = {
    .group_name        = "main",
    .test_name         = "003_unicode",
    .description_short = "sample test description",
    .description       = "long test description",
    .expected_failure  = false,
    .func              = main_003_unicode
};
