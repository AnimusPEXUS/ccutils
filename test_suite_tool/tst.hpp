#ifndef WAYROUND_I2P_20240516_194621_144523
#define WAYROUND_I2P_20240516_194621_144523

#include <algorithm>
#include <any>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include <experimental/scope>

namespace wayround_i2p::ccutils::tst
{

enum LoggerMSGType : unsigned char
{
    Status,
    Info,
    Warning,
    Error,
    Failure,
    Success
};

std::string icon_by_type(LoggerMSGType);

struct TSTInfo;

struct TSTFuncOpts
{
    TSTInfo                        &func_info;
    std::map<std::string, std::any> ingroup_inter_test_memory;

    void Log(TSTInfo &, LoggerMSGType, std::string);
};

struct TSTFuncResult
{
    bool test_success = false;
};

using TST_TEST_FUNCTION = std::function<TSTFuncResult(const TSTFuncOpts &)>;

struct TSTInfo
{
    std::string group_name;
    std::string test_name;
    std::string description_short;
    std::string description;

    bool expected_failure = false;

    TST_TEST_FUNCTION func;
};

struct GroupsMapItem
{
    std::vector<std::string>       test_order;
    std::map<std::string, TSTInfo> tests;
};

struct run_tests_Parameters
{
    std::string                          title;
    std::string                          description;
    std::string                          version;
    std::string                          mod_date;
    std::vector<std::string>             group_order;
    std::map<std::string, GroupsMapItem> groups;

    void Log(LoggerMSGType, std::string);
    int  AddTest(TSTInfo info);
};

int run_tests(run_tests_Parameters &tl);

} // namespace wayround_i2p::ccutils::tst

#endif
