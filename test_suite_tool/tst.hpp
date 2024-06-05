#ifndef WAYROUND_I2P_20240601_083133_769759
#define WAYROUND_I2P_20240601_083133_769759

#include <algorithm>
#include <any>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <experimental/scope>

#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::tst
{

enum LoggerMSGType : unsigned char
{
    Status,
    Text,
    Info,
    Warning,
    Error,
    Failure,
    ExpectedFailure,
    Success,
    UnexpectedSuccess
};

wayround_i2p::ccutils::unicode::UString icon_by_type(LoggerMSGType);
wayround_i2p::ccutils::unicode::UString timestamp();

struct TSTInfo;

struct TSTFuncOpts
{
    const TSTInfo &func_info;

    // iitm - ingroup_inter_test_memory
    std::map<std::string, std::any> &iitm;

    void Log(LoggerMSGType, wayround_i2p::ccutils::unicode::UString) const;
};

struct TSTFuncResult
{
    bool test_success = false;
};

using TST_TEST_FUNCTION = std::function<TSTFuncResult(const TSTFuncOpts &)>;

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

struct run_tests_Parameters
{
    wayround_i2p::ccutils::unicode::UString                          title = "(not set)";
    wayround_i2p::ccutils::unicode::UString                          description;
    wayround_i2p::ccutils::unicode::UString                          uri;
    wayround_i2p::ccutils::unicode::UString                          version;
    wayround_i2p::ccutils::unicode::UString                          mod_date;
    std::vector<wayround_i2p::ccutils::unicode::UString>             group_order;
    std::map<wayround_i2p::ccutils::unicode::UString, GroupsMapItem> groups;

    void Log(LoggerMSGType, wayround_i2p::ccutils::unicode::UString) const;
    int  AddTest(const TSTInfo &info);
};

int run_tests(int argc, char **args, const run_tests_Parameters &tl);

} // namespace wayround_i2p::ccutils::tst

#endif
