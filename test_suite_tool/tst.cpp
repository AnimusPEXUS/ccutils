
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

namespace wayround_i2p::ccutils::tst
{

void run_tests_Parameters::Log(LoggerMSGType t, std::string msg)
{
    std::cout << std::format("todo: run_tests_Options::Log") << std::endl;
}

TSTFuncOpts::TSTFuncOpts(const TSTFunctionInfo &func_info) :
    func_info(func_info)
{
}

TSTFuncOpts::~TSTFuncOpts()
{
}

int run_tests(run_tests_Parameters &tlo)
{
    for (auto &group_name : tlo.group_order)
    {

        if (
            auto group_itr = tlo.groups.find(
                group_name
            );
            group_itr == std::end(tlo.groups)
        )
        {
            tlo.Log(Error, std::format("'{}' not in groups", group_name));
            return 1;
        }
        else
        {
            const GroupsMapItem &group = std::get<1>(*group_itr);

            std::map<std::string, std::any> ingroup_inter_test_memory = {};

            for (auto &test_name : group.test_order)
            {
                if (auto test_itr = group.tests.find(
                        test_name
                    );
                    test_itr == std::end(group.tests))
                {
                    tlo.Log(
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
                    const TSTFunctionInfo &x = std::get<1>(*test_itr);

                    TSTFuncOpts opts(x);
                    opts.ingroup_inter_test_memory = ingroup_inter_test_memory;
                    opts.log;
                }
            }
        }
    }

    return 0;
}

} // namespace wayround_i2p::ccutils::tst
