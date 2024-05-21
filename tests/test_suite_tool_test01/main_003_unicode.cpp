wayround_i2p::ccutils::tst::TSTFuncResult main_003_unicode(
    wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{

    auto x = wayround_i2p::ccutils::unicode::UString("тестовая строка");

    opts.Log(
        wayround_i2p::ccutils::tst::Info,
        std::format(
            "проверяем юникод. длинна \"{}\" == {}",
            x.string_utf8(),
            x.size()
        )
    );

    return {
        x.size() == 15
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
