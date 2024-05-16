#ifndef test_head
#define test_head

#include <functional>

namespace wayround_i2p::ccutils::tst
{

enum LoggerMSGType : unsigned char
{
    Status,
    Info,
    Warning,
    Error
};

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

struct TSTFuncOpts
{
    const TSTFunctionInfo                                             &func_info;
    const std::map<std::string, std::any>                              ingroup_storage;
    std::function<const TSTFunctionInfo &, LoggerMSGType, std::string> log;
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

struct run_tests_Options
{
    std::vector<string> group_order;
    map < std::string,
        struct
    {
        std::vector<string> test_order;
        map<
            std::string,
            TSTFunctionInfo &>
            tests;
    }
        > groups;
};

int run_tests(run_tests_Options &tl);

} // namespace wayround_i2p::ccutils::tst

#endif
