
#include "getopt.hpp"

namespace wayround_i2p::ccutils::cliapp
{

GetOptResult::GetOptResult(const AppCmdNode &node_info)
    : node_info(node_info)
{
}

ccu_uni::UString GetOptResult::String()
{
    ccu_uni::UString ret = "";

    for (std::size_t i = 0; i < this->opts.size(); i++)
    {
        auto &t1 = this->opts[i];

        ret += t1->name;
        if (t1->has_value)
        {
            ret += std::format("=%s", t1->value);
        }
        ret += "\n";
    }

    for (std::size_t i = 0; i < this->args.size(); i++)
    {
        auto &t1 = this->args[i];

        ret += std::format("'%s'\n", t1);
    }

    return ret;
}

std::shared_ptr<RetOptItem> GetOptResult::GetLastNamedRetOptItem(ccu_uni::UString name)
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

std::deque<std::shared_ptr<RetOptItem>> GetOptResult::GetAllNamedRetOptItems(ccu_uni::UString name)
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

std::deque<ccu_err::error_ptr> &GetOptResult::Check(std::deque<ccu_err::error_ptr> &err)
{
    err.clear();

    // check opts required but absent

    for (auto &i : this->node_info.options->items)
    {
        if (i.is_required)
        {
            auto res = this->GetLastNamedRetOptItem(i.name);
            if (!res)
            {
                err.push_back(
                    ccu_err::New(
                        std::format("option %s is required but absent, i.name"),
                        __FILE__,
                        __LINE__
                    )
                );
            }
        }

        // check option's values which must have values
        if (i.must_have_value)
        {
            auto item = this->GetLastNamedRetOptItem(i.name);
            if (item && !item->has_value)
            {
                if (i.has_default_value)
                {
                    item->has_value = true;
                    item->value     = i.default_value;
                }
                else
                {
                    err.push_back(
                        ccu_err::New(
                            std::format("option %s must have value", i.name),
                            __FILE__,
                            __LINE__
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

        for (auto &ii : this->node_info.options->items)
        {
            if (ii.name == i->name)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            err.push_back(
                ccu_err::New(
                    std::format("unsupported option passed: %s", i->name),
                    __FILE__,
                    __LINE__
                )
            );
        }
    }

    // check count of given args
    // todo: check this
    if (this->node_info.check_args)
    {
        auto len_args = this->args.size();

        if (len_args < this->node_info.min_args)
        {
            err.push_back(
                ccu_err::New(
                    "given too few arguments",
                    __FILE__,
                    __LINE__
                )
            );
        }

        if (this->node_info.min_args > -1 && len_args > this->node_info.max_args)
        {
            err.push_back(
                ccu_err::New(
                    "given too few arguments",
                    __FILE__,
                    __LINE__
                )
            );
        }
    }

    return err;
}

bool GetOptResult::DoesHaveNamedRetOptItem(ccu_uni::UString name)
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

bool GetOptResult::DoesNamedRetOptItemHaveValue(ccu_uni::UString name)
{
    if (this->DoesHaveNamedRetOptItem(name))
    {
        return this->GetLastNamedRetOptItem(name)->has_value;
    }
    return false;
}

/* this is not in go version and I don't remember why this is here.
const GetOptCheckListItem &GetOptResult::GetNodeOptionByName(ccu_uni::UString name)
{
}
*/

const GetOptCheckListItem *GetOptCheckList::GetByName(ccu_uni::UString name) const
{
    for (auto &i : this->items)
    {
        if (i.name == name)
        {
            return &i;
        }
    }
    return nullptr;
}

ccu_uni::UString GetOptCheckListItem::HelpString() const
{
    return "Help String (todo)";
}

ccu_uni::UString GetOptCheckList::HelpString() const
{
    return "Help String (todo)";
}

std::shared_ptr<GetOptResult>
    getopt(
        int               argc,
        char            **args,
        const AppCmdNode &node_info,
        bool              args_includes_arg0,
        ccu_uni::UString  arg0
    )
{
    std::deque<ccu_uni::UString> params;

    for (std::size_t i = 0; i <= argc; i++)
    {
        params.push_back(args[i]);
    }

    if (args_includes_arg0)
    {
        arg0 = params[0];
        params.pop_front();
    }

    return getopt(params, node_info, false, arg0);
}

} // namespace wayround_i2p::ccutils::cliapp
