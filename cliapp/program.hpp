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

// make read-only (const?)
struct AppResult
{
    int              code;
    ccu_uni::UString message;
    bool             do_not_print_result;
};

struct AppCmdNode;

// make read-only (const?)
struct AdditionalInfo
{
    std::vector<ccu_uni::UString> opts_and_args;
    GetOptCheckList               available_options;
    std::vector<ccu_uni::UString> depth_level;
    std::shared_ptr<AppCmdNode>   subnode;
    std::shared_ptr<AppCmdNode>   root;
    ccu_uni::UString              arg0;
};

struct GetOptResult;

// make read-only (const?)
struct AppCmdNode
{
    ccu_uni::UString                         name;
    ccu_uni::UString                         short_description;
    ccu_uni::UString                         version;
    ccu_uni::UString                         versionInfo;
    ccu_uni::UString                         devStatus;
    ccu_uni::UString                         description;
    ccu_uni::UString                         license;
    ccu_uni::UString                         date;
    std::vector<ccu_uni::UString>            developers;
    std::vector<ccu_uni::UString>            URIs;
    std::vector<ccu_uni::UString>            man_pages;
    std::vector<ccu_uni::UString>            info_pages;
    std::vector<std::shared_ptr<AppCmdNode>> sub_cmds;

    std::function<
        std::shared_ptr<AppResult>(
            std::shared_ptr<GetOptResult>   getopt_result,
            std::shared_ptr<AdditionalInfo> additional_info
        )>
        callable;

    GetOptCheckList available_options;

    bool cahck_args;
    int  min_args;
    int  max_args;
};

// returns 0 if no errors
int CheckAppCmdNode(std::shared_ptr<AppCmdNode> in, std::deque<ccu_uni::UString> depth);

// note: in Golang version, this function does not return,
//       but in this c++ version it does.
int RunApp(
    std::shared_ptr<AppCmdNode> rootnode,
    std::any                    pass_data
);

std::shared_ptr<AppResult> RunCmd(
    ccu_uni::UString             arg0,
    std::deque<ccu_uni::UString> opts_and_args,
    std::shared_ptr<AppCmdNode>  rootnode,
    std::any                     pass_data
);

} // namespace wayround_i2p::ccutils::cliapp

#endif
