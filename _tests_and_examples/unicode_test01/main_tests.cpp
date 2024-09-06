#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    wayround_i2p::ccutils::unicode::UString x = "тест";

    logger->Log(
        wayround_i2p::ccutils::logger::Status,
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

// -----------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_002(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    return {false};
}

wayround_i2p::ccutils::tst::TSTInfo main_002_i = {
    .group_name = "main",
    .test_name  = "002",
    .func       = main_002
};
