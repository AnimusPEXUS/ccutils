#include <wayround_i2p/ccutils/ip/ip.hpp>

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    return {false};
}

wayround_i2p::ccutils::tst::TSTInfo main_001_i = {
    .group_name        = "main",
    .test_name         = "001",
    .description_short = "testing ip strings parsing",
    .func              = main_001
};
