

#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

#include "tests_main.cpp"

int main(int argc, char **args)
{

    wayround_i2p::ccutils::tst::run_tests_Parameters params;

    {
        auto logger = wayround_i2p::ccutils::logger::TerminalLogger::create();

        params.title  = "WayRound.I2P ccutils::ip test suite";
        params.uri    = "https://github.com/AnimusPEXUS/ccutils";
        params.logger = logger;

        params.AddTest(main_000_i);
        params.AddTest(main_001_i);
        params.AddTest(main_002_i);
        params.AddTest(main_003_i);
    }

    return wayround_i2p::ccutils::tst::run_tests(argc, args, params);
}
