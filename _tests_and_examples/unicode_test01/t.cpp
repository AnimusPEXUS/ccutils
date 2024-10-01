
#include <wayround_i2p/ccutils/errors/e.hpp>

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
                std::cout << "exception: " << e->ErrorLong() << std::endl;
            }

            std::set_terminate(nullptr);
            std::terminate();
        }
    );

    auto logger = wayround_i2p::ccutils::logger::TerminalLogger::create();

    wayround_i2p::ccutils::tst::run_tests_Parameters params;

    params.title  = "WayRound.I2P ccutils::unicode test suite";
    params.uri    = "https://github.com/AnimusPEXUS/ccutils";
    params.logger = logger;

    params.AddTest(main_std_format_i);
    params.AddTest(main_string_center_i);
    params.AddTest(main_string_count_i);
    params.AddTest(main_string_startswith_i);
    params.AddTest(main_string_endswith_i);
    params.AddTest(main_string_expandtabs_i);
    params.AddTest(main_string_index_i);
    params.AddTest(main_string_upper_isUpper_i);
    params.AddTest(main_string_capitalize_i);
    params.AddTest(main_string_title_i);
    params.AddTest(main_string_strip_i);
    params.AddTest(main_string_partition_i);
    params.AddTest(main_string_removeprefix_i);
    params.AddTest(main_string_replace_i);
    params.AddTest(main_string_ljust_i);
    params.AddTest(main_string_rjust_i);
    params.AddTest(main_string_split_i);
    params.AddTest(main_string_splitlines_i);
    params.AddTest(main_string_substr_i);

    return wayround_i2p::ccutils::tst::run_tests(argc, args, params);
}
