
#include <deque>

#include <wayround_i2p/ccutils/cliapp/program.hpp>
#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

namespace ccu_cliapp = wayround_i2p::ccutils::cliapp;
namespace ccu_uni    = wayround_i2p::ccutils::unicode;

constexpr ccu_cliapp::AppCmdNode app_struct{
    .name "testapp",
    cmd_options : {}
};

static_assert(ccu_cliapp::validate_AppCmdNode(app_struct));

int main(int argc, char **args)
{
    std::deque<ccu_uni::UString> opts_and_args;

    opts_and_args = ccu_cliapp::convert_libc_args(
        argc,
        args,
        opts_and_args
    );

    auto res = ccu_cliapp::RunCmd(
        opts_and_args[0],
        opts_and_args,
        app_struct,
        0
    );

    return res->code;
}
