#ifndef WAYROUND_I2P_20240711_143336_990951
#define WAYROUND_I2P_20240711_143336_990951

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

using TST_TEST_FUNCTION = std::function<TSTFuncResult(
    const TSTInfo                                &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
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

struct run_tests_Parameters
{
    wayround_i2p::ccutils::unicode::UString                          title = "(not set)";
    wayround_i2p::ccutils::unicode::UString                          description;
    wayround_i2p::ccutils::unicode::UString                          uri;
    wayround_i2p::ccutils::unicode::UString                          version;
    wayround_i2p::ccutils::unicode::UString                          mod_date;
    std::vector<wayround_i2p::ccutils::unicode::UString>             group_order;
    std::map<wayround_i2p::ccutils::unicode::UString, GroupsMapItem> groups;

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
        wayround_i2p::ccutils::logger::LoggerMSGType   t,
        const wayround_i2p::ccutils::unicode::UString &msg
    ) const;

    void Log(
        wayround_i2p::ccutils::logger::LoggerMSGType               t,
        const std::deque<wayround_i2p::ccutils::unicode::UString> &msg
    ) const;

    void LogSplitLines(
        wayround_i2p::ccutils::logger::LoggerMSGType   t,
        const wayround_i2p::ccutils::unicode::UString &msg
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
