
#include "getopt.hpp"

namespace wayround_i2p::ccutils::cliapp
{

ccu_uni::UString GetOptResult::String()
{
    ccu_uni::UString ret = "";

    for (std::size_t i = 0; i < this->opts.size(); i++)
    {
        auto &t1 = this->opts[i];

        ret += t1->name;
        if (t1->have_value)
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

std::deque<ccu_err::error_ptr> GetOptResult::CheckOptResult()
{
}

bool GetOptResult::DoesHaveNamedRetOptItem(ccu_uni::UString name)
{
}

bool GetOptResult::DoesNamedRetOptItemHaveValue(ccu_uni::UString name)
{
}

const GetOptCheckListItem &GetOptResult::GetNodeOptionByName(ccu_uni::UString name)
{
}

const GetOptCheckListItem &GetByName(
    const GetOptCheckList &lst,
    ccu_uni::UString       name
)
{
}

ccu_uni::UString HelpString(const GetOptCheckList &lst)
{
}

} // namespace wayround_i2p::ccutils::cliapp
