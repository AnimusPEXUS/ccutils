
#include "program.hpp"

namespace wayround_i2p::ccutils::cliapp
{

ccu_uni::UString AppCmdNodeOptionsItem::HelpString() const
{
    return "Help String (todo)";
}

ccu_uni::UString AppCmdNodeOptions::HelpString() const
{
    return "Help String (todo)";
}

std::optional<const AppCmdNodeOptionsItem *> AppCmdNodeOptions::GetByName(ccu_uni::UString name) const
{
    for (auto &i : this->items)
    {
        if (i->name == name)
        {
            return i;
        }
    }
    return nullptr;
}

std::shared_ptr<const AppResult> RunCmd(
    const ccu_uni::UString             &arg0,
    const std::deque<ccu_uni::UString> &opts_and_args,
    const AppCmdNode                   &rootnode,
    std::any                            pass_data
)
{
    auto ret = std::make_shared<AppResult>();

    ret->code                = 0;
    ret->message             = "No Error";
    ret->do_not_print_result = false;

    std::deque<ccu_uni::UString> depth_level;
    const AppCmdNode            *subtree;

    auto getopt_result = getopt(opts_and_args, rootnode, false, arg0);

    // searching subtree and/or command

    // todo: perform rootnode sanity checks
    // auto check_result = getopt_result.Check(std::deque<ccu_err::error_ptr>());

    subtree = &rootnode;

    depth_level.clear();

    for (const auto &i : getopt_result->args)
    {
        for (const auto &j : subtree->sub_cmds)
        {
            if (j.name == i)
            {
                subtree = &j;
                depth_level.push_back(i);

                if (subtree->callable)
                {
                    goto search_success;
                }
                goto continue_look_for_requested_cmd_or_subtree;
            }
        }

        ret->code                = 100;
        ret->message             = "arguments error: invalid command or subsection name";
        ret->do_not_print_result = false;
        return ret;

    continue_look_for_requested_cmd_or_subtree:
    }

    // at this point search considered successfull

search_success:
    // showing help or executing command

    auto help_option = getopt_result->GetLastNamedRetOptItem("--help");
    if (help_option)
    {
        if (help_option->has_value)
        {
            ret->code                = 1;
            ret->message             = "options error: option --help does not assume additional value";
            ret->do_not_print_result = false;
            return ret;
        }

        PrintHelp(depth_level, rootnode, *subtree);
        ret->code                = 0;
        ret->message             = "printing help text";
        ret->do_not_print_result = false;
        return ret;
    }

    if (!subtree->callable)
    {
        ret->code                = 100;
        ret->message             = "not callable";
        ret->do_not_print_result = false;
        return ret;
    }

    getopt_result->node_info = subtree;

    auto additional_info = std::make_shared<AppCmdRuntimeInfo>();

    ret = subtree->callable(
        getopt_result,
        additional_info
    );

    if (!ret)
    {
        ret = std::make_shared<AppResult>();

        ret->code = 0;
    }

    if (ret->code == 0 && ret->message == "")
    {
        ret->message = "No Error";
    }

    return ret;
}

void PrintHelp(
    const std::deque<ccu_uni::UString> depth_level,
    const AppCmdNode                  &rootnode,
    const AppCmdNode                  &render_for
)
{
}

} // namespace wayround_i2p::ccutils::cliapp
