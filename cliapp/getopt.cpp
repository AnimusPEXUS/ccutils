
#include "getopt.hpp"

namespace wayround_i2p::ccutils::cliapp
{

ccu_uni::UString GetOptResult::String() const
{
    ccu_uni::UString ret = "";

    for (std::size_t i = 0; i < this->opts.size(); i++)
    {
        auto &t1 = this->opts[i];

        ret += t1->name;
        if (t1->has_value)
        {
            ret += std::format("={}", t1->value);
        }
        ret += "\n";
    }

    for (std::size_t i = 0; i < this->args.size(); i++)
    {
        auto &t1 = this->args[i];

        ret += std::format("'{}'\n", t1);
    }

    return ret;
}

std::shared_ptr<RetOptItem> GetOptResult::GetLastNamedRetOptItem(ccu_uni::UString name) const
{
    std::shared_ptr<RetOptItem> ret;
    for (auto i : this->opts)
    {
        if (i->name == name)
        {
            ret = i;
        }
    }
    return ret;
}

std::deque<std::shared_ptr<RetOptItem>> GetOptResult::GetAllNamedRetOptItems(ccu_uni::UString name) const
{
    std::deque<std::shared_ptr<RetOptItem>> ret;
    for (auto i : this->opts)
    {
        if (i->name == name)
        {
            ret.push_back(i);
        }
    }
    return ret;
}

std::deque<ccu_err::error_ptr> &GetOptResult::Check(
    const AppCmdNode               *node_to_check_against,
    std::deque<ccu_err::error_ptr> &err
) const
{
    err.clear();

    // check opts required but absent

    assert(node_to_check_against);

    for (auto i : node_to_check_against->options->items)
    {
        if (i->is_required)
        {
            auto res = this->GetLastNamedRetOptItem(i->name);
            if (!res)
            {
                err.push_back(
                    ccu_err::New(
                        std::format(
                            "option {} is required but absent",
                            i->name
                        )
                    )
                );
            }
        }

        // check option's values which must have values
        if (i->must_have_value)
        {
            auto item = this->GetLastNamedRetOptItem(i->name);
            if (item && !item->has_value)
            {
                if (i->has_default_value)
                {
                    item->has_value = true;
                    item->value     = i->default_value;
                }
                else
                {
                    err.push_back(
                        ccu_err::New(
                            std::format(
                                "option {} must have value",
                                i->name
                            )
                        )
                    );
                }
            }
        }
    }

    // check if unsupported options passed
    for (auto &i : this->opts)
    {
        bool found = false;

        for (auto ii : node_to_check_against->options->items)
        {
            if (ii->name == i->name)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            err.push_back(
                ccu_err::New(
                    std::format(
                        "unsupported option passed: {}",
                        i->name
                    )
                )
            );
        }
    }

    // check count of given args
    // todo: check this
    if (node_to_check_against->check_args)
    {
        auto len_args = this->args.size();

        if (len_args < node_to_check_against->min_args)
        {
            err.push_back(
                ccu_err::New(
                    "given too few arguments"
                )
            );
        }

        if (node_to_check_against->min_args > -1
            && len_args > node_to_check_against->max_args)
        {
            err.push_back(
                ccu_err::New(
                    "given too few arguments"
                )
            );
        }
    }

    return err;
}

bool GetOptResult::DoesHaveNamedRetOptItem(ccu_uni::UString name) const
{
    for (auto &i : this->opts)
    {
        if (i->name == name)
        {
            return true;
        }
    }
    return false;
}

bool GetOptResult::DoesNamedRetOptItemHaveValue(ccu_uni::UString name) const
{
    if (this->DoesHaveNamedRetOptItem(name))
    {
        return this->GetLastNamedRetOptItem(name)->has_value;
    }
    return false;
}

std::shared_ptr<GetOptResult>
    getopt(
        int              argc,
        char           **args,
        bool             args_includes_arg0,
        ccu_uni::UString arg0
    )
{
    auto params = std::make_shared<std::deque<ccu_uni::UString>>();

    for (std::size_t i = 0; i <= argc; i++)
    {
        params->push_back(args[i]);
    }

    if (args_includes_arg0)
    {
        arg0 = params->operator[](0);
        params->pop_front();
    }

    return getopt(params, false, arg0);
}

std::shared_ptr<GetOptResult>
    getopt(
        std::shared_ptr<std::deque<ccu_uni::UString>> params,
        bool                                          params_includes_arg0,
        ccu_uni::UString                              arg0
    )
{
    auto ret             = std::make_shared<GetOptResult>();
    bool last_opt_switch = false;
    int  len_args        = 0;
    int  i               = 0;
    int  args_i_len      = 0;
    int  eq_pos          = 0;

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

    len_args = params->size();

    i = 0;

    while (true)
    {
        if (i >= len_args)
        {
            break;
        }

        auto params_i = params->begin()[i];

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

std::shared_ptr<std::deque<ccu_uni::UString>> convert_libc_args(
    int    argc,
    char **args
)
{
    auto ret = std::make_shared<std::deque<ccu_uni::UString>>();
    for (std::size_t i = 0; i < argc; i++)
    {
        ret->push_back(args[i]);
    }
    return ret;
}

} // namespace wayround_i2p::ccutils::cliapp
