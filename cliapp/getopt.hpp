#ifndef WAYROUND_I2P_20260724_233235_2589840000
#define WAYROUND_I2P_20260724_233235_2589840000

#include <memory>

#include "_package.hpp"

namespace wayround_i2p::ccutils::cliapp
{

namespace ccu_uni = wayround_i2p::ccutils::unicode;
namespace ccu_err = wayround_i2p::ccutils::errors;

struct RetOptItem;
struct AppCmdNode;
struct GetOptCheckListItem;

struct GetOptResult
{
    const AppCmdNode                             &node_info;
    const std::deque<ccu_uni::UString>            args;
    const std::deque<std::shared_ptr<RetOptItem>> opts;

    ccu_uni::UString String();

    // If arguments contained many options with name 'name', then return only last
    // of them
    std::shared_ptr<RetOptItem> GetLastNamedRetOptItem(ccu_uni::UString name);

    // return all options with name 'name'
    std::deque<std::shared_ptr<RetOptItem>> GetAllNamedRetOptItems(ccu_uni::UString name);

    // check result accordingly with AvailableOptions passed to
    std::deque<ccu_err::error_ptr> CheckOptResult();

    // check if option with name 'name' has been supplied
    bool DoesHaveNamedRetOptItem(ccu_uni::UString name);

    // check if option with name 'name' has been supplied and it have some value
    bool DoesNamedRetOptItemHaveValue(ccu_uni::UString name);

    const GetOptCheckListItem &GetNodeOptionByName(ccu_uni::UString name);
};

struct RetOptItem
{
    ccu_uni::UString name;
    bool             have_value;
    ccu_uni::UString value;
};

// getopt configuration
struct GetOptCheckListItem
{
    /*
        must be with leading '-' (minuses) and without trailing '=' equals
    */
    const ccu_uni::UString name;

    /*
        set this to true if Default value have meaning
    */
    const bool has_default_value;

    /*
        default value in case if defined without value
    */
    const ccu_uni::UString default_value;

    /*
        put here true, if user MUST explicitly define this option
    */
    const bool is_required;

    /* insist on value to flag.
    if this is true, and value not passed to option,
    then if HaveDefault is true then Default is used as value.
    */
    const bool must_have_value;

    const ccu_uni::UString description; // this text will be printed beside option on --help parameter
};

struct GetOptCheckList
{
    GetOptCheckListItem items[];
};

std::shared_ptr<GetOptCheckListItem> GetByName(
    ccu_uni::UString       name,
    const GetOptCheckList &lst
);

ccu_uni::UString HelpString(const GetOptCheckList &lst);

} // namespace wayround_i2p::ccutils::cliapp

#endif
