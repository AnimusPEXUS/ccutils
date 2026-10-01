
#include "program.hpp"

namespace wayround_i2p::ccutils::cliapp
{

/* this has to be in compile-time
int CheckAppCmdNode(
    const AppCmdNode                   &in,
    const std::deque<ccu_uni::UString> &depth
)
{
}
*/

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
    AppCmdNode                  *subtree;

    getopt_result = getopt();

    {
        check();

        loop through getopt.args
        {
            loop through subcommands
            {
                if arg
                    == subcommand name
                    {
                        save subcommand                 as subtree;
                        append subcommand name to depth level;
            in go version here was check for subtree.Callable != nil:
                           if true - stop searching and exit the loops;
            if
                false - continue looking;
            if loop
                anded and nothing found - return error;
            but this is wrong - needs fixing;
                    }
            }
        }
    }

    if everything
        is still ok, try to find-- help option.

                         if show_help
                         == true,
            then
        {
            PrintHelp(depth_level, rootnode, subtree);
            ret = &AppResult{0, "help text printed", false};
        }
}

std::deque<ccu_uni::UString> &convert_libc_args(
    int                           argc,
    char                        **args,
    std::deque<ccu_uni::UString> &ret
)
{
    ret.clear();
    for (std::size_t i = 0; i < argc; i++)
    {
        ret.push_back(args[i]);
    }
    return ret;
}

} // namespace wayround_i2p::ccutils::cliapp
