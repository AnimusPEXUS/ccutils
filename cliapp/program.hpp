#ifndef WAYROUND_I2P_20260724_233235_2588710000
#define WAYROUND_I2P_20260724_233235_2588710000

#include <any>
#include <functional>
#include <vector>

#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

#include "_package.hpp"

namespace wayround_i2p::ccutils::cliapp
{

namespace ccu_uni = wayround_i2p::ccutils::unicode;
namespace ccu_err = wayround_i2p::ccutils::errors;

struct AppResult
{
    int              code;
    ccu_uni::UString message;
    bool             do_not_print_result;
};

struct AppCmdNode;
struct AppCmdNodeOptions;
struct AppCmdNodeOptionsItem;

struct AppCmdRuntimeInfo
{
    std::shared_ptr<const std::deque<ccu_uni::UString>> params;
    std::optional<const AppCmdNodeOptions *>            options;
    std::shared_ptr<std::deque<const AppCmdNode *>>     depth_level;
    const ccu_uni::UString                              arg0;
    std::any                                            user_data;
};

struct GetOptResult;

struct AppCmdNodeInfo
{
    ccu_uni::UString              shortDescription;
    ccu_uni::UString              version;
    ccu_uni::UString              versionInfo;
    ccu_uni::UString              devStatus;
    ccu_uni::UString              description;
    ccu_uni::UString              argumentsDescription;
    ccu_uni::UString              license;
    ccu_uni::UString              date;
    std::vector<ccu_uni::UString> developers;
    std::vector<ccu_uni::UString> URIs;
    std::vector<ccu_uni::UString> man_pages;
    std::vector<ccu_uni::UString> info_pages;
};

struct AppCmdNode
{
    // basic part
    ccu_uni::UString name;

    const AppCmdNodeInfo *info;

    // section part

    std::vector<AppCmdNode *> subnodes;

    // command part

    bool check_args;
    int  min_args;
    int  max_args;

    std::function<std::shared_ptr<AppResult>(
        std::shared_ptr<const GetOptResult>      getopt_result,
        std::shared_ptr<const AppCmdRuntimeInfo> runtime_info
    )>
        callable;

    const AppCmdNodeOptions                         *options;
    bool                                             error_on_unsupported_options = true;
    const std::vector<const AppCmdNodeOptionsItem *> required_options;

    bool is_required_option(const AppCmdNodeOptionsItem *val);
};

std::shared_ptr<std::deque<ccu_err::error_ptr>> AppCmdNode_Validate(
    const AppCmdNode                               *node,
    std::deque<ccu_uni::UString>                    depth,
    std::shared_ptr<std::deque<ccu_err::error_ptr>> ret
);

std::shared_ptr<std::deque<ccu_err::error_ptr>> AppCmdNode_Validate_and_Print(
    const AppCmdNode *node
);

struct AppCmdNodeOptionsItem
{
    /*
        must be with leading '-' (minuses) (more than 2 is allowed)
        and without trailing '=' (equals)
    */
    const ccu_uni::UString name;

    /*
        set this to true if option allows value
    */
    const bool accepts_value = false;

    /*
        default value if value not supplied by user
    */
    const ccu_uni::UString default_value = "";

    /* insist on value to flag.
    if this is true, and value not passed to option,
    then if HaveDefault is true then Default is used as value.
    */
    const bool must_have_value = false;

    /*
    what to print after `=' in --help text
    (if allows_value is true, then inline_descr must be not empty)
    */
    const ccu_uni::UString inline_descr = "";

    /*
    this text will be printed beside option on --help parameter
    */
    const ccu_uni::UString description = "";

    ccu_uni::UString HelpString() const;
};

// todo: confirm this really is required to be separate class, and not just vector.
struct AppCmdNodeOptions
{
    const std::vector<const AppCmdNodeOptionsItem *> items;

    const AppCmdNodeOptionsItem *GetByName(
        ccu_uni::UString name
    ) const;

    // ccu_uni::UString HelpString() const;
};

/*
constexpr bool validate_AppCmdNode(const AppCmdNode &node_data)
{
    if (!(node_data.min_args >= -1))
    {
        return false;
    }

    if (!(node_data.max_args >= -1))
    {
        return false;
    }

    if (!(node_data.max_args >= node_data.min_args))
    {
        return false;
    }

    if (!(node_data.callable != nullptr))
    {
        return false;
    }

    return true;
}
*/

/*
// returns 0 if no errors
int CheckAppCmdNode(
    const AppCmdNode                   &in,
    const std::deque<ccu_uni::UString> &depth
);
*/

std::shared_ptr<const AppResult> RunCmd(
    const ccu_uni::UString                       &arg0,
    std::shared_ptr<std::deque<ccu_uni::UString>> params,
    const AppCmdNode                             &rootnode,
    const std::any                                user_data
);

int PrintAppResult(std::shared_ptr<const AppResult> in);

void PrintHelp(
    std::shared_ptr<std::deque<const AppCmdNode *>> depth_level
);

void PrintDepthLevel(
    std::shared_ptr<std::deque<const AppCmdNode *>> depth_level,
    ccu_uni::UString                                separator
);

ccu_uni::UString DepthLevelToString(
    std::shared_ptr<std::deque<const AppCmdNode *>> depth_level,
    ccu_uni::UString                                separator
);

} // namespace wayround_i2p::ccutils::cliapp

#endif
