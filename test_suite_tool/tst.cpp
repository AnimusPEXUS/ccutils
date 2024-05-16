
#include <wayround_i2p/ccutils/test_suite_tool/tdt.hpp>

namespace wayround_i2p::ccutils::tst
{

std::shared_ptr<Logger> Logger::create()
{
    auto ret = std::shared_ptr<Logger>(new Logger());
    return ret;
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::Log(
    const TSTFunctionInfo &func_info,
    LoggerMSGType          t,
    std::string            msg
)
{
}

int run_tests(run_tests_Options &tlo)
{

    for (auto &group_name : tlo.group_order)
    {

        if (
            auto group_itr = tlo.find(tlo.groups.begin(), tlo.groups.end(), group_name);
            group_itr == tlo.groups.end()
        )
        {
            tlo.logger.Log(Error, std::format("'{}' not in groups", group_name));
            return 1;
        }
        else
        {
            auto &group = group_itr.get();

            std::map<std::string, std::any> ingroup_inter_test_memory = {};

            for (auto &test_name : group.test_order)
            {
                if (auto test_itr = tlo.find(group.tests.begin(), group.tests.end(), test_name);
                    test_itr == group.tests.end())
                {
                    tlo.logger.Log(
                        Error,
                        std::format(
                            "'{}' not in tests of group",
                            test_name,
                            group_name
                        )
                    );
                    return 2;
                }
                else
                {
                    TSTFunctionInfo &x = test_itr.get();
                }
            }
        }
    }

    return 0;
}

} // namespace wayround_i2p::ccutils::tst
