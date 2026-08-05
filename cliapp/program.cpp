
#include "program.hpp"

namespace wayround_i2p::ccutils::cliapp
{

int CheckAppCmdNode(
    const AppCmdNode                   &in,
    const std::deque<ccu_uni::UString> &depth
)
{
}

int RunApp(
    const AppCmdNode &rootnode,
    std::any          pass_data
)
{
}

std::shared_ptr<const AppResult> RunCmd(
    const ccu_uni::UString             &arg0,
    const std::deque<ccu_uni::UString> &opts_and_args,
    const AppCmdNode                   &rootnode,
    std::any                            pass_data
)
{
}

} // namespace wayround_i2p::ccutils::cliapp
