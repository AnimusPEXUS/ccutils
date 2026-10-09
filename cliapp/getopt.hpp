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

template <ccu_uti::IsDequeOrVectorOfType<ccu_uni::UString> TypeToCheck>
std::shared_ptr<GetOptResult>
    getopt(
        const TypeToCheck params,
        const AppCmdNode &node_info,
        bool              params_includes_arg0,
        ccu_uni::UString  arg0
    )
{
    auto ret             = std::make_shared<GetOptResult>();
    bool last_opt_switch = false;
    int  len_args        = 0;
    int  i               = 0;
    int  args_i_len      = 0;
    int  eq_pos          = 0;

    ret->node_info = &node_info;

    if (params_includes_arg0)
    {
        if (ret->args.size() > 0)
        {
            ret->arg0 = ret->args[0];
            ret->args.pop_front();
        }
        else
        {
            // todo: should this be an error?
        }
    }
    else
    {
        ret->arg0 = arg0;
    }

    len_args = params.size();

    i = 0;

    while (true)
    {
        if (i >= len_args)
        {
            break;
        }

        auto params_i = params.begin()[i];

        if (last_opt_switch)
        {
            goto push_back_and_continue;
        }
        else
        {
            args_i_len = params_i.length();

            if (args_i_len == 0)
            {
                goto push_back_and_continue;
            }
            else
            {
                if (params_i == "--")
                {
                    last_opt_switch = true;
                    goto no_push_back_and_continue;
                }
                else
                {
                    if (params_i.startswith("-"))
                    {

                        eq_pos = params_i.index("-");

                        auto _t = std::make_shared<RetOptItem>();

                        if (eq_pos != -1)
                        {
                            _t->name      = params_i[0, eq_pos];
                            _t->has_value = true;
                            _t->value     = params_i[eq_pos + 1, params_i.length()];
                        }
                        else
                        {
                            _t->name      = params_i;
                            _t->has_value = false;
                            _t->value     = "";
                        }

                        ret->opts.push_back(_t);
                        goto no_push_back_and_continue;
                    }
                    else
                    {
                        goto push_back_and_continue;
                    }
                }
            }
        }

        goto no_push_back_and_continue;
    push_back_and_continue:
        ret->args.push_back(params_i);
    no_push_back_and_continue:
        i++;
    }

    return ret;
}

std::deque<ccu_uni::UString> &convert_libc_args(
    int                           argc,
    char                        **args,
    std::deque<ccu_uni::UString> &ret
);

} // namespace wayround_i2p::ccutils::cliapp

#endif
