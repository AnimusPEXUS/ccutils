
#include "program.hpp"

namespace wayround_i2p::ccutils::cliapp
{

bool AppCmdNode::is_required_option(const AppCmdNodeOptionsItem *val)
{
    for (auto i : required_options)
    {
        if (i == val)
        {
            return true;
        }
    }
    return false;
}

// todo: make this validation compile-time?
std::shared_ptr<std::deque<ccu_err::error_ptr>> AppCmdNode_Validate(
    const AppCmdNode                               *node,
    std::deque<ccu_uni::UString>                    depth,
    std::shared_ptr<std::deque<ccu_err::error_ptr>> ret
)
{
    if (depth.size() == 0)
    {
        if (node->name == "")
        {
            ret->push_back(
                ccu_err::New(
                    "root node must have defined name"_ccUS
                )
            );
        }
    }

    depth.push_back(node->name);

    // todo: check memory usage at join(*depth)
    auto depth_str = ", "_ccUS.join(depth);

    if (node->callable && node->subnodes.size() != 0)
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

    if (!node->callable && node->subnodes.size() == 0)
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

    if (node->min_args < -1)
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

    if (node->max_args < -1)
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

    if (node->max_args < node->min_args)
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

    for (std::size_t i = 0; i < node->subnodes.size(); i++)
    {
        ret = AppCmdNode_Validate(
            node,
            depth,
            ret
        );
    }

    if (node->options)
    {
        auto opts = node->options;
        { // find invalid option names

            for (auto i : node->options->items)
            {
                auto n = i->name;
                if (!n.startswith("-"_ccUS) || n.endswith("="_ccUS))
                {
                    ret->push_back(
                        ccu_err::New(
                            std::format(
                                "invalid option name `{}' at: {}",
                                n,
                                depth_str
                            )
                        )
                    );
                }
            }
        }

        { // find duplicated options (by name)

            std::deque<ccu_uni::UString> name_list;

            for (auto i : node->options->items)
            {
                auto n = i->name;

                bool found = false;

                for (auto j : name_list)
                {
                    if (j == n)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    ret->push_back(
                        ccu_err::New(
                            std::format(
                                "found duplicated option ({}) at: {}",
                                n,
                                depth_str
                            )
                        )
                    );
                }
                else
                {
                    name_list.push_back(n);
                }
            }
        }
    }

    {
        {
            auto opts = node->options;

            if (node->required_options.size() == 0)
            {
                if (!opts)
                {
                    ret->push_back(
                        ccu_err::New(
                            std::format(
                                "values to required_options supplied, but node->options is nullptr. at: {}",
                                depth_str
                            )
                        )
                    );
                }
            }
            else
            {
                for (auto i : node->required_options)
                {
                    bool found = false;
                    for (auto j : opts->items)
                    {
                        if (j == i)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        ret->push_back(
                            ccu_err::New(
                                std::format(
                                    "item is in node->required_options, but not in node->options. at: {}",
                                    depth_str
                                )
                            )
                        );
                    }
                }
            }
        }

        { // find duplicated node->required_options

            std::deque<const AppCmdNodeOptionsItem *> addrs;

            for (auto i : node->required_options)
            {
                bool found = false;

                for (auto j : addrs)
                {
                    if (j == i)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    ret->push_back(
                        ccu_err::New(
                            std::format(
                                "found duplicated `required' option ({}). at: {}",
                                i->name,
                                depth_str
                            )
                        )
                    );
                }
                else
                {
                    addrs.push_back(i);
                }
            }
        }
    }

    return ret;
}

std::shared_ptr<std::deque<ccu_err::error_ptr>> AppCmdNode_Validate_and_Print(
    const AppCmdNode *node
)
{
    auto ret = AppCmdNode_Validate(
        node,
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
    auto ret = ""_ccUS;

    ret += this->name;

    if (this->accepts_value)
    {

        if (!this->must_have_value)
        {
            ret += "["_ccUS;
        }

        ret += "="_ccUS + (this->inline_descr == ""_ccUS ? "value" : this->inline_descr) + ""_ccUS;

        if (!this->must_have_value)
        {
            ret += "]"_ccUS;
        }
    }

    return ret;
}

/*
ccu_uni::UString AppCmdNodeOptions::HelpString() const
{
    auto ret = ""_ccUS;

    for (auto i = this->items.begin();
         i != this->items.end();
         i++)
    {
        ret += (*i)->HelpString();
        if (i + 1 != this->items.end())
        {
            ret += " "_ccUS;
        }
    }

    return ret;
}
*/

const AppCmdNodeOptionsItem *AppCmdNodeOptions::GetByName(
    ccu_uni::UString name
) const
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
    const ccu_uni::UString                       &arg0,
    std::shared_ptr<std::deque<ccu_uni::UString>> params,
    const AppCmdNode                             &rootnode,
    const std::any                                user_data
)
{

    auto ret = std::make_shared<AppResult>(
        AppResult{
            .code                = 0,
            .do_not_print_result = false
        }
    );

    auto depth_level = std::make_shared<std::deque<const AppCmdNode *>>();

    const AppCmdNode *curren_node = nullptr;

    auto getopt_result = getopt(params, false, arg0);

    // todo: perform rootnode sanity checks
    // auto check_result = getopt_result.Check(std::deque<ccu_err::error_ptr>());

    curren_node       = &rootnode;
    std::size_t level = 0;

    depth_level->push_back(curren_node);

    // searching subtree and/or command (new)

subnode_search_loop:
{
    if (curren_node->callable)
    {
        goto search_success;
    }

    if (getopt_result->args.size() <= level)
    {
        goto search_success;
    }

    auto next_name_to_search = getopt_result->args[level];
    for (const auto subnode : curren_node->subnodes)
    {
        if (subnode->name == next_name_to_search)
        {
            level++;
            curren_node = subnode;
            depth_level->push_back(curren_node);
            goto subnode_search_loop;
        }
    }

    ret->code                = 4;
    ret->message             = "arguments error: invalid command or subsection name";
    ret->do_not_print_result = false;
    return ret;
}

    // at this point search considered successfull

search_success:

    // remove first 'level' count of args from getopt result
    getopt_result->args.erase(
        getopt_result->args.begin(),
        getopt_result->args.begin() + level
    );

    // showing help or executing command

    auto help_option = getopt_result->GetLastNamedRetOptItem("--help");
    if (help_option)
    {
        if (help_option->has_value)
        {
            ret->code                = 3;
            ret->message             = "options error: option --help does not assume additional value";
            ret->do_not_print_result = false;
            return ret;
        }

        PrintHelp(depth_level);
        ret->code                = 0;
        ret->message             = "printing help text";
        ret->do_not_print_result = false;
        return ret;
    }

    auto dl_size = depth_level->size();

    if (!(depth_level->operator[](dl_size - 1)->callable))
    {
        ret->code                = 2;
        ret->message             = "not callable";
        ret->do_not_print_result = false;
        return ret;
    }

    auto runtime_info = std::make_shared<AppCmdRuntimeInfo>(
        AppCmdRuntimeInfo{
            .params      = params,
            .options     = depth_level->operator[](dl_size - 1)->options,
            .depth_level = depth_level,
            .arg0        = arg0,
            .user_data   = user_data
        }
    );

    ret = depth_level->operator[](dl_size - 1)->callable(getopt_result, runtime_info);

    if (!ret)
    {
        ret = std::make_shared<AppResult>();

        ret->code = 0;
    }

    if (ret->code == 0 && ret->message == "")
    {
        ret->message = "ok";
    }

    return ret;
}

int PrintAppResult(std::shared_ptr<const AppResult> in)
{
    auto ret = in->code;
    if (!in->do_not_print_result)
    {
        std::cout
            << std::format(
                   "exit code: {} ({})",
                   ret,
                   in->message
               )
            << std::endl;
    }
    return ret;
}

void PrintHelp(
    std::shared_ptr<std::deque<const AppCmdNode *>> depth_level
)
{
    if (depth_level->size() == 0)
    {
        throw ccu_err::New("depth_level not allowed to be empty here");
    }

    auto root       = depth_level->begin()[0];
    auto render_for = depth_level->end()[-1];

    auto root_info       = root->info;
    auto render_for_info = render_for->info;

    ccu_uni::UString text;

    text += root->name;

    {
        if (render_for_info)
        {
            if (render_for_info->version != ""_ccUS)
            {
                text += " version:\""_ccUS + render_for_info->version + "\""_ccUS;
                if (render_for_info->versionInfo != ""_ccUS)
                {
                    text += " version info: "_ccUS + render_for_info->versionInfo;
                }
            }
        }
    }

    {
        if (render_for_info)
        {
            if (render_for_info->devStatus != ""_ccUS)
            {
                text += " status: "_ccUS + render_for_info->devStatus;
            }
        }
    }

    {
        if (render_for_info)
        {
            if (render_for_info->devStatus != ""_ccUS)
            {
                text += " license: "_ccUS + render_for_info->license;
            }
        }
    }

    {
        if (render_for_info)
        {
            if (render_for_info->date != ""_ccUS)
            {
                text += " date: "_ccUS + render_for_info->date;
            }
        }
    }

    text += "\n\n"_ccUS;

    text += "Usage: "_ccUS;

    for (
        auto i = depth_level->begin();
        i != depth_level->end();
        i++)
    {
        text += (*i)->name;
        if (i != depth_level->end() && (i + 1 != depth_level->end()))
        {
            text += " "_ccUS;
        }
    }

    {
        if (render_for->required_options.size() != 0)
        {
            text += " "_ccUS;
        }

        for (auto i = render_for->required_options.begin();
             i != render_for->required_options.end();
             i++)
        {
            text += (*i)->HelpString();
            if (i + 1 != render_for->required_options.end())
            {
                text += " "_ccUS;
            }
        }

        if (render_for->required_options.size() != 0)
        {
            text += " "_ccUS;
        }

        text += "[other options] [--] [args]"_ccUS;

        text += "\n\n"_ccUS;
    }

    {
        if (render_for_info)
        {

            if (render_for_info->shortDescription != ""_ccUS)
            {
                auto new_descr = render_for_info->shortDescription;
                new_descr      = new_descr.replace("\n"_ccUS, "\n  "_ccUS, -1);

                text += "Short Info:\n  "_ccUS + new_descr;
                text += "\n\n"_ccUS;
            }
        }
    }

    std::deque<AppCmdNode *> subnodes;
    std::deque<AppCmdNode *> subcommands;

    for (auto i : render_for->subnodes)
    {
        if (!i->callable)
        {
            subnodes.push_back(i);
        }
        else
        {
            subcommands.push_back(i);
        }
    }

    if (subnodes.size() != 0)
    {
        text += "SubSections:\n\n"_ccUS;
        for (auto i : subnodes)
        {
            text += " "_ccUS + i->name + "\n"_ccUS;
            {
                auto i_info = i->info;
                if (i_info)
                {
                    if (i_info->shortDescription != ""_ccUS)
                    {
                        text += "  "_ccUS + i_info->shortDescription + "\n"_ccUS;
                    }
                    text += "\n"_ccUS;
                }
            }
        }
    }

    if (subnodes.size() != 0)
    {
        text += "Commands:\n\n"_ccUS;
        for (auto i : subcommands)
        {
            text        += " "_ccUS + i->name + "\n"_ccUS;
            auto i_info  = i->info;
            if (i_info)
            {
                if (i_info->shortDescription != ""_ccUS)
                {
                    auto new_deskr  = i_info->shortDescription;
                    new_deskr       = new_deskr.replace("\n"_ccUS, "\n  "_ccUS, -1);
                    text           += "  "_ccUS + new_deskr + "\n"_ccUS;
                }
                text += "\n"_ccUS;
            }
        }
    }

    if (render_for->callable)
    {
        text += "Arguments: "_ccUS;

        if (!render_for->check_args)
        {
            text += "unchecked"_ccUS;
        }
        else
        {

            std::deque<ccu_uni::UString> texts;

            if (render_for->min_args == -1)
            {
                texts.push_back("minimum arguments count - undefined"_ccUS);
            }
            else
            {
                texts.push_back(std::format("minimum arguments count: {}", render_for->min_args));
            }

            if (render_for->max_args == -1)
            {
                texts.push_back("maximum arguments count - undefined"_ccUS);
            }
            else
            {
                texts.push_back(std::format("maximum arguments count: {}", render_for->max_args));
            }

            if ((render_for->max_args == render_for->min_args) && render_for->min_args != -1)
            {
                texts.push_back(std::format("exact count: {}", render_for->min_args));
            }

            text += ", "_ccUS.join(texts) + "."_ccUS;
        }

        text += "\n"_ccUS;

        if (render_for_info)
        {

            if (render_for_info->argumentsDescription != ""_ccUS)
            {
                auto new_descr = render_for_info->argumentsDescription;
                new_descr      = new_descr.replace("\n"_ccUS, "\n  "_ccUS, -1);

                text += "  "_ccUS + new_descr + "\n"_ccUS;
            }
        }
        text += "\n"_ccUS;
    }

    {
        if (render_for_info)
        {
            if (render_for_info->description != ""_ccUS)
            {
                text += "Description:\n "_ccUS + render_for_info->description;
                text += "\n\n"_ccUS;
            }
        }
    }

    // todo: forcibly add doc for '--help' option?

    auto opts = render_for->options;

    // todo: use HelpString() from options?
    if (opts)
    {
        if (opts->items.size() != 0)
        {
            text += "Options:\n\n"_ccUS;

            for (auto i : opts->items)
            {
                text += " "_ccUS;

                text += i->HelpString();

                text += " ("_ccUS
                      + (i->accepts_value ? (
                                                i->must_have_value ?
                                                    "requires value"_ccUS :
                                                    "value is optional (default: `"_ccUS + i->default_value + "')"_ccUS
                                            ) :
                                            "value no accepted"_ccUS)
                      + ")";

                text += "\n"_ccUS;

                if (i->description != ""_ccUS)
                {
                    text += "  "_ccUS + i->description + "\n"_ccUS;
                }

                text += "\n"_ccUS;
            }

            text += "\n\n"_ccUS;
        }
    }

    {
        if (render_for_info)
        {
            auto man_pages_size = render_for_info->man_pages.size();
            if (man_pages_size != 0)
            {
                text += "manpage"_ccUS;
                if (man_pages_size > 1)
                {
                    text += "s"_ccUS;
                }
                text += ":\n"_ccUS;
                text += " "_ccUS + ", "_ccUS.join(render_for_info->man_pages) + "\n"_ccUS;
                text += "\n"_ccUS;
            }
        }
    }

    {
        if (render_for_info)
        {
            auto info_pages_size = render_for_info->info_pages.size();
            if (info_pages_size != 0)
            {
                text += "infopage"_ccUS;
                if (info_pages_size > 1)
                {
                    text += "s"_ccUS;
                }
                text += ":\n"_ccUS;
                text += " "_ccUS + ", "_ccUS.join(render_for_info->info_pages) + "\n"_ccUS;
                text += "\n"_ccUS;
            }
        }
    }

    {
        if (render_for_info)
        {
            auto developers_size = render_for_info->developers.size();
            if (developers_size != 0)
            {
                text += "Developer"_ccUS;
                if (developers_size > 1)
                {
                    text += "s"_ccUS;
                }
                text += ":\n"_ccUS;

                for (auto i : render_for_info->developers)
                {
                    text += " "_ccUS + i + "\n"_ccUS;
                }
                text += "\n"_ccUS;
            }
        }
    }

    {
        if (render_for_info)
        {
            auto uris_size = render_for_info->URIs.size();
            if (uris_size != 0)
            {
                text += "URI"_ccUS;
                if (uris_size > 1)
                {
                    text += "s"_ccUS;
                }
                text += ":\n"_ccUS;

                for (auto i : render_for_info->URIs)
                {
                    text += " "_ccUS + i + "\n"_ccUS;
                }
                text += "\n"_ccUS;
            }
        }
    }

    std::cout << text;
    std::cout.flush();
}

void PrintDepthLevel(
    std::shared_ptr<std::deque<const AppCmdNode *>> depth_level,
    ccu_uni::UString                                separator
)
{
    std::cout << DepthLevelToString(
        depth_level,
        separator
    );
}

ccu_uni::UString DepthLevelToString(
    std::shared_ptr<std::deque<const AppCmdNode *>> depth_level,
    ccu_uni::UString                                separator
)
{
    auto dl_s = depth_level->size();

    std::vector<ccu_uni::UString> lst(dl_s);

    for (std::size_t i = 0; i < dl_s; i++)
    {
        lst[i] = depth_level->operator[](i)->name;
    }

    return separator.join(lst);
}

} // namespace wayround_i2p::ccutils::cliapp
