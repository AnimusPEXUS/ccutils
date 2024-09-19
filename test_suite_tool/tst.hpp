#ifndef WAYROUND_I2P_20240718_170059_660333
#define WAYROUND_I2P_20240718_170059_660333

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

using UString = wayround_i2p::ccutils::unicode::UString;

struct TSTInfo;

struct TSTFuncResult
{
    bool test_success = false;
};

using TST_TEST_FUNCTION = std::function<TSTFuncResult(
    const TSTInfo                                &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)>;

struct TSTInfo
{
    UString group_name;
    UString test_name;
    UString description_short;
    UString description;
    bool    description_print;

    bool expected_failure = false;

    TST_TEST_FUNCTION func;
};

struct GroupsMapItem
{
    std::vector<UString>               test_order;
    std::map<UString, const TSTInfo &> tests;
};

struct run_tests_Parameters
{
    UString                          title = "(not set)";
    UString                          description;
    UString                          uri;
    UString                          version;
    UString                          mod_date;
    std::vector<UString>             group_order;
    std::map<UString, GroupsMapItem> groups;

    wayround_i2p::ccutils::logger::LoggerI_shared logger;

    int AddTest(const TSTInfo &info);
};

class IndividualFunctionLogger : public wayround_i2p::ccutils::logger::LoggerI
{
  public:
    static std::shared_ptr<IndividualFunctionLogger> create(
        const run_tests_Parameters &params,
        const TSTInfo              &test_info
    );

    void Log(
        wayround_i2p::ccutils::logger::LoggerMSGType t
    ) const;

    void Log(
        wayround_i2p::ccutils::logger::LoggerMSGType t,
        const UString                               &msg
    ) const;

    void Log(
        wayround_i2p::ccutils::logger::LoggerMSGType t,
        const std::deque<UString>                   &msg
    ) const;

    void LogSplitLines(
        wayround_i2p::ccutils::logger::LoggerMSGType t,
        const UString                               &msg
    ) const;

    ~IndividualFunctionLogger();

  protected:
    IndividualFunctionLogger(
        const run_tests_Parameters &params,
        const TSTInfo              &test_info
    );

  private:
    const run_tests_Parameters &params;
    const TSTInfo              &test_info;
};

int run_tests(int argc, char **args, const run_tests_Parameters &tl);

} // namespace wayround_i2p::ccutils::tst

#endif
