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
    std::deque<ccu_uni::UString>             params;
    std::optional<const AppCmdNodeOptions *> cmd_options;
    std::vector<ccu_uni::UString>            depth_level;
    std::optional<const AppCmdNode *>        parent_node;
    std::optional<const AppCmdNode *>        current_node;
    std::optional<const AppCmdNode *>        root;
    const ccu_uni::UString                   arg0;
    std::any                                 user_data;
};

struct GetOptResult;

struct AppCmdNodeInfo
{
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
};

struct AppCmdNode
{
    // basic part
    ccu_uni::UString name;

    AppCmdNodeInfo *info;

    // section part

    std::vector<AppCmdNode *> subnodes;

    // command part

    bool check_args;
    int  min_args;
    int  max_args;

    std::function<std::shared_ptr<AppResult>(
        std::shared_ptr<GetOptResult>      getopt_result,
        std::shared_ptr<AppCmdRuntimeInfo> additional_info
    )>
        callable;

    const AppCmdNodeOptions *options;
};

std::shared_ptr<std::deque<ccu_err::error_ptr>> AppCmdNode_Validate(
    const AppCmdNode                               *node_info,
    std::deque<ccu_uni::UString>                    depth,
    std::shared_ptr<std::deque<ccu_err::error_ptr>> ret
);

std::shared_ptr<std::deque<ccu_err::error_ptr>> AppCmdNode_Validate_and_Print(
    const AppCmdNode *node_info
);

struct AppCmdNodeOptionsItem
{
    /*
        must be with leading '-' (minuses) and without trailing '=' equals
    */
    const ccu_uni::UString name;

    /*
        set this to true if Default value have meaning
    */
    const bool has_default_value = false;

    /*
        default value in case if defined without value
    */
    const ccu_uni::UString default_value = "";

    /*
        put here true, if user MUST explicitly define this option
    */
    const bool is_required = false;

    /* insist on value to flag.
    if this is true, and value not passed to option,
    then if HaveDefault is true then Default is used as value.
    */
    const bool must_have_value = false;

    const ccu_uni::UString description = ""; // this text will be printed beside option on --help parameter

    ccu_uni::UString HelpString() const;
};

struct AppCmdNodeOptions
{
    const std::vector<const AppCmdNodeOptionsItem *> items;

    std::optional<const AppCmdNodeOptionsItem *> GetByName(
        ccu_uni::UString name
    ) const;

    ccu_uni::UString HelpString() const;
};

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

// returns 0 if no errors
int CheckAppCmdNode(
    const AppCmdNode                   &in,
    const std::deque<ccu_uni::UString> &depth
);

std::shared_ptr<const AppResult> RunCmd(
    const ccu_uni::UString                             &arg0,
    const std::shared_ptr<std::deque<ccu_uni::UString>> params,
    const AppCmdNode                                   &rootnode,
    const std::any                                      user_data
);

void PrintHelp(
    const std::deque<ccu_uni::UString> depth_level,
    const AppCmdNode                  &rootnode,
    const AppCmdNode                  &render_for
);

} // namespace wayround_i2p::ccutils::cliapp

#endif
