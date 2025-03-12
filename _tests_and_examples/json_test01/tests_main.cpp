#include <algorithm>
#include <vector>

#include <wayround_i2p/ccutils/json/json.hpp>

namespace json = wayround_i2p::ccutils::json;

using UString = wayround_i2p::ccutils::unicode::UString;

std::vector<UString> tst_txt = []()
{
    std::vector<UString> ret;

    ret.push_back(R"--(

{ "a": 123 }

)--");

    return ret;
}();

// -----------------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_JSON_test01(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    for (auto &i : tst_txt)
    {
        auto res = json::json_PATTERN()->match(i);
        logger->LogSplitLines(
            wayround_i2p::ccutils::logger::Status,
            res->repr_as_text(true)
        );
    }

    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_IP_create_i = {
    .group_name = "main",
    .test_name  = "JSON_test01",
    .func       = main_JSON_test01
};
