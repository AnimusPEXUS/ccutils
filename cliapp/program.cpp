
#include "program.hpp"

namespace wayround_i2p::ccutils::cliapp
{

std::shared_ptr<std::deque<ccu_err::error_ptr>> AppCmdNode_Validate(
    const AppCmdNode                               *node_info,
    std::deque<ccu_uni::UString>                    depth,
    std::shared_ptr<std::deque<ccu_err::error_ptr>> ret
)
{
    if (depth.size() == 0)
    {
        if (node_info->name == "")
        {
            ret->push_back(
                ccu_err::New(
                    std::format(
                        "root node must have defined name"
                    )
                )
            );
        }
    }

    depth.push_back(node_info->name);

    // todo: check memory usage at join(*depth)
    auto depth_str = ccu_uni::UString(", ").join(depth);

    if (node_info->callable && node_info->subnodes.size() != 0)
    {
        ret->push_back(
            ccu_err::New(
                std::format(
                    "{}: node can't be callable and have subnodes at the same time",
                    depth_str
                )
            )
        );
    }

    if (!node_info->callable && node_info->subnodes.size() == 0)
    {
        ret->push_back(
            ccu_err::New(
                std::format(
                    "{}: node must be callable or have subnodes",
                    depth_str
                )
            )
        );
    }

    if (node_info->min_args < -1)
    {
        ret->push_back(
            ccu_err::New(
                std::format(
                    "min_args must be >= -1. path: {}",
                    depth_str
                )
            )
        );
    }

    if (node_info->max_args < -1)
    {
        ret->push_back(
            ccu_err::New(
                std::format(
                    "max_args must be >= -1. path: {}",
                    depth_str
                )
            )
        );
    }

    if (node_info->max_args < node_info->min_args)
    {
        ret->push_back(
            ccu_err::New(
                std::format(
                    "max_args must be >= min_args. path: {}",
                    depth_str
                )
            )
        );
    }

    for (std::size_t i = 0; i < node_info->subnodes.size(); i++)
    {
        ret = AppCmdNode_Validate(
            node_info,
            depth,
            ret
        );
    }

    return ret;
}

std::shared_ptr<std::deque<ccu_err::error_ptr>> AppCmdNode_Validate_and_Print(
    const AppCmdNode *node_info
)
{
    auto ret = AppCmdNode_Validate(
        node_info,
        std::deque<ccu_uni::UString>(),
        std::make_shared<std::deque<ccu_err::error_ptr>>()
    );

    for (auto i : *ret)
    {
        std::cerr
            << std::format(
                   "AppCmdNode error: {}",
                   i->Error()
               )
            << std::endl;
    }

    return ret;
}

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
    const ccu_uni::UString                             &arg0,
    const std::shared_ptr<std::deque<ccu_uni::UString>> params,
    const AppCmdNode                                   &rootnode,
    const std::any                                      user_data
)
{
    auto ret = std::make_shared<AppResult>();

    ret->code                = 0;
    ret->message             = "No Error";
    ret->do_not_print_result = false;

    std::deque<ccu_uni::UString> depth_level;
    const AppCmdNode            *subtree;

    auto getopt_result = getopt(params, rootnode, false, arg0);

    // searching subtree and/or command

    // todo: perform rootnode sanity checks
    // auto check_result = getopt_result.Check(std::deque<ccu_err::error_ptr>());

    subtree = &rootnode;

    depth_level.clear();

    for (const auto &i : getopt_result->args)
    {
        for (const auto j : subtree->subnodes)
        {
            if (j->name == i)
            {
                subtree = j;
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

    additional_info->user_data = user_data;

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
