#ifndef WAYROUND_I2P_20260724_233235_2589840000
#define WAYROUND_I2P_20260724_233235_2589840000

#include <memory>

#include <initializer_list>

#include "_package.hpp"

#include <wayround_i2p/ccutils/utils/concepts.hpp>

namespace wayround_i2p::ccutils::cliapp
{

namespace ccu_uni = wayround_i2p::ccutils::unicode;
namespace ccu_err = wayround_i2p::ccutils::errors;
namespace ccu_uti = wayround_i2p::ccutils::utils;

struct RetOptItem;
struct AppCmdNode;
struct GetOptCheckListItem;

struct GetOptResult
{
    // GetOptResult(AppCmdNode &node_info);

    std::optional<const AppCmdNode *>       node_info;
    ccu_uni::UString                        arg0;
    std::deque<ccu_uni::UString>            args;
    std::deque<std::shared_ptr<RetOptItem>> opts;

    ccu_uni::UString String() const;

    // If arguments contained many options with name 'name', then return only last
    // of them
    std::shared_ptr<RetOptItem> GetLastNamedRetOptItem(ccu_uni::UString name) const;

    // return all options with name 'name'
    std::deque<std::shared_ptr<RetOptItem>> GetAllNamedRetOptItems(ccu_uni::UString name) const;

    // check result accordingly with AvailableOptions passed to
    std::deque<ccu_err::error_ptr> &Check(std::deque<ccu_err::error_ptr> &err) const;

    // check if option with name 'name' has been supplied
    bool DoesHaveNamedRetOptItem(ccu_uni::UString name) const;

    // check if option with name 'name' has been supplied and it have some value
    bool DoesNamedRetOptItemHaveValue(ccu_uni::UString name) const;
};

struct RetOptItem
{
    ccu_uni::UString name;
    bool             has_value;
    ccu_uni::UString value;
};

// if args doen't contains arg 0, arg 0 should be provided as arg0
std::shared_ptr<GetOptResult>
    getopt(
        int               argc,
        char            **args,
        const AppCmdNode &node_info,
        bool              args_includes_arg0 = true,
        ccu_uni::UString  arg0               = ""
    );

std::shared_ptr<GetOptResult>
    getopt(
        const std::shared_ptr<std::deque<ccu_uni::UString>> params,
        const AppCmdNode                                   &node_info,
        bool                                                params_includes_arg0,
        ccu_uni::UString                                    arg0
    );

const std::shared_ptr<std::deque<ccu_uni::UString>> convert_libc_args(
    int    argc,
    char **args
);

} // namespace wayround_i2p::ccutils::cliapp

#endif
