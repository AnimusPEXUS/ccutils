#ifndef WAYROUND_I2P_20240516_194621_144523
#define WAYROUND_I2P_20240516_194621_144523

#include <algorithm>
#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace wayround_i2p::ccutils::tst
{

enum LoggerMSGType : unsigned char
{
    Status,
    Info,
    Warning,
    Error
};

/*
// todo: use (put this, if it's not exists, to) ccutils::logger instead
class Logger
{
  public:
    static std::shared_ptr<Logger> create();

  protected:
    Logger();

  public:
    ~Logger();

    void Log(const TSTFunctionInfo &, LoggerMSGType, std::string);

  private:
}
*/

struct TSTFunctionInfo;

struct TSTFuncOpts
{
    TSTFuncOpts(const TSTFunctionInfo &func_info);
    ~TSTFuncOpts();

    const TSTFunctionInfo                                                   &func_info;
    std::map<std::string, std::any>                                          ingroup_inter_test_memory;
    std::function<void(const TSTFunctionInfo &, LoggerMSGType, std::string)> log;
};

struct TSTFuncResult
{
    bool test_success = false;
};

using TST_TEST_FUNCTION = std::function<TSTFuncResult(const TSTFuncOpts &)>;

struct TSTFunctionInfo
{
    std::string name;
    std::string description_teaser;
    std::string description;

    bool fail_is_ok = false;

    TST_TEST_FUNCTION func;
};

struct GroupsMapItem
{
    std::vector<std::string>                       test_order;
    std::map<std::string, const TSTFunctionInfo &> tests;
};

struct run_tests_Options
{
    std::vector<std::string>                     group_order;
    std::map<std::string, const GroupsMapItem &> groups;

    void Log(LoggerMSGType, std::string);
};

int run_tests(run_tests_Options &tl);

} // namespace wayround_i2p::ccutils::tst

#endif
