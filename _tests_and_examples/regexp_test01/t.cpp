

#include <wayround_i2p/ccutils/logger/logger.hpp>
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

#include "main_tests.cpp"

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

        params.title  = "WayRound.I2P ccutils::regexp test suite";
        params.uri    = "https://github.com/AnimusPEXUS/ccutils";
        params.logger = logger;

        params.AddTest(main_001_i);
        params.AddTest(main_002_i);
        params.AddTest(main_003_i);
        params.AddTest(main_004_i);
        params.AddTest(main_005_i);
        params.AddTest(main_006_i);
        params.AddTest(main_008_i);
        params.AddTest(main_009_i);
        params.AddTest(main_010_i);
    }

    return wayround_i2p::ccutils::tst::run_tests(argc, args, params);
}
