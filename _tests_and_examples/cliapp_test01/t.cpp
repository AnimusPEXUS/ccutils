
#include <cassert>
#include <deque>

#include <wayround_i2p/ccutils/cliapp/program.hpp>
#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

namespace ccu_cliapp = wayround_i2p::ccutils::cliapp;
namespace ccu_uni    = wayround_i2p::ccutils::unicode;

const ccu_cliapp::AppCmdNodeOptionsItem test_opt1{
    .name        = "--test-opt1",
    .description = "testing option number 1"
};

const ccu_cliapp::AppCmdNodeOptionsItem test_opt2{
    .name        = "--test-opt2",
    .description = "testing option number 2"
};

const ccu_cliapp::AppCmdNodeOptionsItem test_opt3{
    .name          = "--test-opt3",
    .accepts_value = true,
    .description   = "testing option number 3"
};

/*
const ccu_cliapp::AppCmdNodeOptionsItem cli_main_menu_item_help{
    .name        = "--help",
    .description = "show help"
};
*/

const ccu_cliapp::AppCmdNodeOptions cli_main_menu_options{
    .items = {
              &test_opt1,
              &test_opt2,
              &test_opt3
    }
};

std::shared_ptr<ccu_cliapp::AppResult> f1(
    std::shared_ptr<const ccu_cliapp::GetOptResult>      getopt_result,
    std::shared_ptr<const ccu_cliapp::AppCmdRuntimeInfo> runtime_info
)
{
    auto ret = std::make_shared<ccu_cliapp::AppResult>();
    std::cout << "test" << std::endl;
    return ret;
}

const ccu_cliapp::AppCmdNodeInfo cli_main_menu_info{
    .shortDescription     = "short single-line description",
    .version              = "v0.1",
    .versionInfo          = "version info",
    .devStatus            = "development",
    .description          = "long\n multiline\n description",
    .argumentsDescription = "args description (multiline)",
    .license              = "GPLv3",
    .date                 = "2026-10-30",
    .developers           = {"I am", "I am", "I am"},
    .URIs                 = {"https://github.com/AnimusPEXUS/ccutils/cliapp"},
    .man_pages            = {"man(1)"},
    .info_pages           = {"info"}
};

const ccu_cliapp::AppCmdNode cli_main_menu{
    .name             = "testapp",
    .info             = &cli_main_menu_info,
    .callable         = f1,
    .options          = &cli_main_menu_options,
    .required_options = {
                         &test_opt2,
                         &test_opt3
    }
};

int main(int argc, char **args)
{
    {
        auto menu_validation_result = AppCmdNode_Validate_and_Print(&cli_main_menu);
        if (menu_validation_result->size() != 0)
        {
            std::abort();
        }
    }

    auto params = ccu_cliapp::convert_libc_args(
        argc,
        args
    );

    auto arg0 = params->operator[](0);
    params->pop_front();

    auto res = ccu_cliapp::RunCmd(
        arg0,
        params,
        cli_main_menu,
        0
    );

    return ccu_cliapp::PrintAppResult(res);
}
