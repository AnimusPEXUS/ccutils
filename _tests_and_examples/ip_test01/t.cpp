

#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

#include "tests_main.cpp"

int main(int argc, char **args)
{

    std::set_terminate(
        []() -> void
        {
            try
            {
                throw;
            }
            catch (const wayround_i2p::ccutils::errors::error_ptr &e)
            {
                std::cout << "exception: " << e->Error() << std::endl;
            }

            std::set_terminate(nullptr);
            std::terminate();
        }
    );

    wayround_i2p::ccutils::tst::run_tests_Parameters params;

    {
        auto logger = wayround_i2p::ccutils::logger::TerminalLogger::create();

        params.title  = "WayRound.I2P ccutils::ip test suite";
        params.uri    = "https://github.com/AnimusPEXUS/ccutils";
        params.logger = logger;

        params.AddTest(main_print_examples_i);
        params.AddTest(main_regexps_tests_i);
        // params.AddTest(main_regexps_shortcuts_tests_i);
        params.AddTest(main_IP_create_i);
    }

    return wayround_i2p::ccutils::tst::run_tests(argc, args, params);
}
