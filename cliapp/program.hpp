#ifndef WAYROUND_I2P_20260724_233235_2588710000
#define WAYROUND_I2P_20260724_233235_2588710000

#include <any>
#include <functional>
#include <vector>

#include <wayround_i2p/ccutils/unicode/u.hpp>

#include "_package.hpp"

namespace wayround_i2p::ccutils::cliapp
{

namespace ccu_uni = wayround_i2p::ccutils::unicode;

struct AppResult
{
    int              code;
    ccu_uni::UString message;
    bool             do_not_print_result;
};

struct AppCmdNode;
struct GetOptCheckList;

struct AppCmdRuntimeInfo
{
    const std::vector<ccu_uni::UString> opts_and_args;
    const GetOptCheckList              &cmd_options;
    const std::vector<ccu_uni::UString> depth_level;
    const AppCmdNode                   &parent_node;
    const AppCmdNode                   &current_node;
    const AppCmdNode                   &root;
    const ccu_uni::UString              arg0;
};

struct GetOptResult;

// instances of this struct must be only compile-time and const
struct AppCmdNode
{
    const ccu_uni::UString              name;
    const ccu_uni::UString              short_description;
    const ccu_uni::UString              version;
    const ccu_uni::UString              versionInfo;
    const ccu_uni::UString              devStatus;
    const ccu_uni::UString              description;
    const ccu_uni::UString              license;
    const ccu_uni::UString              date;
    const std::vector<ccu_uni::UString> developers;
    const std::vector<ccu_uni::UString> URIs;
    const std::vector<ccu_uni::UString> man_pages;
    const std::vector<ccu_uni::UString> info_pages;
    const std::vector<AppCmdNode>       sub_cmds;

    const std::function<std::shared_ptr<AppResult>(
        std::shared_ptr<GetOptResult>      getopt_result,
        std::shared_ptr<AppCmdRuntimeInfo> additional_info
    )>
        callable;

    const std::vector<GetOptCheckList> cmd_options;

    const bool check_args;
    const int  min_args;
    const int  max_args;
};

// returns 0 if no errors
int CheckAppCmdNode(
    const AppCmdNode                   &in,
    const std::deque<ccu_uni::UString> &depth
);

// note: in Golang version, this function does not return,
//       but in this c++ version it does.
int RunApp(
    const AppCmdNode &rootnode,
    const std::any    pass_data
);

std::shared_ptr<const AppResult> RunCmd(
    const ccu_uni::UString             &arg0,
    const std::deque<ccu_uni::UString> &opts_and_args,
    const AppCmdNode                   &rootnode,
    const std::any                      pass_data
);

} // namespace wayround_i2p::ccutils::cliapp

#endif
