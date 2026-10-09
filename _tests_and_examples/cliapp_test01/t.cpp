
#include <deque>

#include <wayround_i2p/ccutils/cliapp/program.hpp>
#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

namespace ccu_cliapp = wayround_i2p::ccutils::cliapp;
namespace ccu_uni    = wayround_i2p::ccutils::unicode;

const ccu_cliapp::AppCmdNodeOptionsItem cli_main_menu_item_help{
    .name        = "--help",
    .description = "show help"
};

const ccu_cliapp::AppCmdNodeOptions cli_main_menu_options{
    .items = {
              &cli_main_menu_item_help,
              }
};

const ccu_cliapp::AppCmdNode cli_main_menu{
    .name    = "testapp",
    .options = &cli_main_menu_options
};

// static_assert(ccu_cliapp::validate_AppCmdNode(&cli_main_menu_options));

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
        cli_main_menu,
        0
    );

    return res->code;
}
