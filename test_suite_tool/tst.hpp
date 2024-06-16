#ifndef WAYROUND_I2P_20240616_100728_824555
#define WAYROUND_I2P_20240616_100728_824555

#include <algorithm>
#include <any>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <experimental/scope>

#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::tst
{

struct TSTInfo;

struct TSTFuncResult
{
    bool test_success = false;
};

using LOGGER_CB_FUNCTION_TYPE = wayround_i2p::ccutils::logger::LoggerFunctionCB_T;

using TST_TEST_FUNCTION = std::function<TSTFuncResult(
    const TSTInfo                   &func_info,
    std::map<std::string, std::any> &iitm,
    LOGGER_CB_FUNCTION_TYPE          logger
)>;

struct TSTInfo
{
    wayround_i2p::ccutils::unicode::UString group_name;
    wayround_i2p::ccutils::unicode::UString test_name;
    wayround_i2p::ccutils::unicode::UString description_short;
    wayround_i2p::ccutils::unicode::UString description;

    bool expected_failure = false;

    TST_TEST_FUNCTION func;
};

struct GroupsMapItem
{
    std::vector<wayround_i2p::ccutils::unicode::UString>               test_order;
    std::map<wayround_i2p::ccutils::unicode::UString, const TSTInfo &> tests;
};

struct run_tests_Parameters : public wayround_i2p::ccutils::logger::LoggerI
{
    wayround_i2p::ccutils::unicode::UString                          title = "(not set)";
    wayround_i2p::ccutils::unicode::UString                          description;
    wayround_i2p::ccutils::unicode::UString                          uri;
    wayround_i2p::ccutils::unicode::UString                          version;
    wayround_i2p::ccutils::unicode::UString                          mod_date;
    std::vector<wayround_i2p::ccutils::unicode::UString>             group_order;
    std::map<wayround_i2p::ccutils::unicode::UString, GroupsMapItem> groups;

    wayround_i2p::ccutils::logger::LoggerI_shared logger;

    void Log(
        wayround_i2p::ccutils::logger::LoggerMSGType,
        wayround_i2p::ccutils::unicode::UString
    ) const;
    int AddTest(const TSTInfo &info);
};

int run_tests(int argc, char **args, const run_tests_Parameters &tl);

} // namespace wayround_i2p::ccutils::tst

#endif
