wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    wayround_i2p::ccutils::tst::TSTFuncOpts &opts
)
{
    wayround_i2p::ccutils::unicode::UString x = "тест";

    opts.Log(
        wayround_i2p::ccutils::tst::Status,
        std::format("test string is \"{}\": it's length is {}", x, x.length())
    );

    return {
        x.length() == 4
    };
}

wayround_i2p::ccutils::tst::TSTInfo main_001_i = {
    .group_name = "main",
    .test_name  = "001",
    .func       = main_001
};
