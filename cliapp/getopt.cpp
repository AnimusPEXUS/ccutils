
#include "getopt.hpp"

namespace wayround_i2p::ccutils::cliapp
{

UString GetOptResult::String()
{
    UString ret = "";

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

std::shared_ptr<RetOptItem> GetOptResult::GetLastNamedRetOptItem(UString name)
{
}

std::deque<std::shared_ptr<RetOptItem>> GetOptResult::GetAllNamedRetOptItems(UString name)
{
}

std::deque<std::shared_ptr<const error>> GetOptResult::CheckOptResult()
{
}

bool GetOptResult::DoesHaveNamedRetOptItem(UString name)
{
}

bool GetOptResult::DoesNamedRetOptItemHaveValue(UString name)
{
}

std::shared_ptr<GetOptCheckListItem> GetOptResult::GetOptionByName(UString name)
{
}

std::shared_ptr<GetOptCheckListItem> GetOptCheckListItem::GetByName(UString name)
{
}

UString GetOptCheckListItem::HelpString()
{
}

} // namespace wayround_i2p::ccutils::cliapp
