
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

namespace wayround_i2p::ccutils::tst
{

std::string icon_by_type(LoggerMSGType t)
{
    std::string icon;

    switch (t)
    {
        default:
            icon = "???";
            break;
        case Status:
            icon = "-~-";
            break;
        case Info:
            icon = "-i-";
            break;
        case Warning:
            icon = "-!-";
            break;
        case Error:
            icon = "-E-";
            break;
        case Failure:
            icon = "-F-";
            break;
        case Success:
            icon = "-s-";
            break;
    }

    return icon;
}

std::string timestamp()
{
    auto t = std::chrono::utc_clock::now();
    return std::format("{0:%F}T{0:%T}z", t);
}

void TSTFuncOpts::Log(TSTInfo &i, LoggerMSGType t, std::string msg)
{
    std::cout << std::format(
        "[{}] [{}] [{}:{}] {}",
        icon_by_type(t),
        timestamp(),
        i.group_name,
        i.test_name,
        msg
    )
              << std::endl;
}

void run_tests_Parameters::Log(
    LoggerMSGType t,
    std::string   msg
)
{
    std::cout << std::format(
        "[{}] [{}] {}",
        icon_by_type(t),
        timestamp(),
        msg
    )
              << std::endl;
}

int run_tests_Parameters::AddTest(
    TSTInfo info
)
{
    if (std::find(
            this->group_order.begin(),
            this->group_order.end(),
            info.group_name
        )
        == this->group_order.end())
    {
        // std::cout << "registering new group name: " << info.group_name
        //          << std::endl;
        this->group_order.push_back(info.group_name);
    }
    else
    {
        //      std::cout << "group name " << info.group_name << " already registered" << std::endl;
    }

    if (this->groups.find(info.group_name) == this->groups.end())
    {
        //        std::cout << "registering new group: " << info.group_name << std::endl;
        this->groups[info.group_name] = {};
    }
    else
    {
        /*
            std::cout << "group " << info.group_name << " already registered" << std::endl;
    */
    }

    auto &grp = this->groups[info.group_name];

    if (std::find(
            grp.test_order.begin(),
            grp.test_order.end(),
            info.test_name
        )
        == grp.test_order.end())
    {
        /*std::cout
            << "registering new test " << info.test_name
            << " in group " << info.group_name
            << std::endl;*/
        grp.test_order.push_back(info.test_name);
    }
    else
    {
        /*        std::cout << "test \"" << info.test_name << "\" already registered in group "
                          << info.group_name << std::endl; */
    }

    if (grp.tests.find(info.test_name) == grp.tests.end())
    {
        /*std::cout
            << "inserting new test info " << info.test_name
            << " in group " << info.group_name
            << std::endl;*/
        grp.tests[info.test_name] = info;
        // std::cout << "grp.tests.size(): " << grp.tests.size() << std::endl;
    }
    else
    {
        std::cerr << std::format(
            " trying to redefine {}:{}",
            info.group_name,
            info.test_name
        ) << std::endl;
        return 1;
    }

    return 0;
}

void print_head(run_tests_Parameters &tlo)
{
    std::string for_string = "WayRound.I2P test suite tool";
    std::string for_string_line;
    std::string desc_string;
    std::string ver_string;
    std::string mod_string;

    if (tlo.title != "")
    {
        for_string = std::format(
            "{} (being tested by {})",
            tlo.title,
            for_string
        );
    }

    for_string = "  " + for_string;

    for (auto i = 0; i != for_string.size() + 2; i++)
    {
        for_string_line += "-";
    }

    if (tlo.description != "")
    {
        desc_string = std::format(
            "description    |   {}   |\n",
            tlo.description
        );
    }

    if (tlo.version != "")
    {
        ver_string = std::format(
            "version        |   {}   |\n",
            tlo.version
        );
    }

    if (tlo.mod_date != "")
    {
        mod_string = std::format(
            "modified date  |   {}   |\n",
            tlo.mod_date
        );
    }

    std::cout << std::format(
        R"+++({0:}
{1}
{0:}
{2}{3}{4})+++",
        for_string_line,
        for_string,
        desc_string,
        ver_string,
        mod_string
    ) << std::endl;
}

int run_tests(run_tests_Parameters &rtp)
{
    int ret = 1;

    unsigned int total_count = 0;

    std::vector<TSTInfo *> success_list;
    std::vector<TSTInfo *> failure_list;
    std::vector<TSTInfo *> expected_failure_list;
    std::vector<TSTInfo *> unexpected_success_list;

    auto se01 = std::experimental::scope_exit(
        [&]()
        {
            std::cout << std::format(R"+++(
total test count: {}
             success: {}
            failures: {}
   expected failures: {}
  unexpected success: {}
)+++",
                                     total_count,
                                     success_list.size(),
                                     failure_list.size(),
                                     expected_failure_list.size(),
                                     unexpected_success_list.size());
            std::cout << "test suite tool exit code: " << ret << std::endl;
            return;
        }
    );

    print_head(rtp);

    rtp.Log(Status, std::format("{} test group(s) provided", rtp.group_order.size()));

    for (auto &group_name : rtp.group_order)
    {
        rtp.Log(Status, std::format("next group is: {}", group_name));

        if (
            auto group_itr = rtp.groups.find(
                group_name
            );
            group_itr == std::end(rtp.groups)
        )
        {
            rtp.Log(Error, std::format("'{}' not in groups", group_name));
            ret = 1;
            return ret;
        }
        else
        {
            GroupsMapItem &group = std::get<1>(*group_itr);

            std::map<std::string, std::any> ingroup_inter_test_memory = {};

            rtp.Log(
                Status,
                std::format(
                    "group {} contains {} test(s)",
                    group_name,
                    group.test_order.size()
                )
            );

            for (auto &test_name : group.test_order)
            {
                if (auto test_itr = group.tests.find(
                        test_name
                    );
                    test_itr == std::end(group.tests))
                {
                    rtp.Log(
                        Error,
                        std::format(
                            "'{}' not in tests of group {}",
                            test_name,
                            group_name
                        )
                    );
                    ret = 2;
                    return ret;
                }
                else
                {
                    rtp.Log(Status, std::format("next test is: {}:{}", group_name, test_name));

                    TSTInfo &x = std::get<1>(*test_itr);

                    TSTFuncOpts opts(x);
                    opts.func_info                 = x;
                    opts.ingroup_inter_test_memory = ingroup_inter_test_memory;

                    total_count++;

                    if (!x.func)
                    {
                        rtp.Log(
                            Error,
                            std::format(
                                "function not defined in '{}:{}'",
                                test_name,
                                group_name
                            )
                        );
                        ret = 3;
                        return ret;
                    }

                    auto res = x.func(opts);
                    if (res.test_success)
                    {
                        if (x.expected_failure)
                        {
                            unexpected_success_list.push_back(&x);
                        }
                        else
                        {
                            success_list.push_back(&x);
                        }
                    }
                    else
                    {
                        if (x.expected_failure)
                        {
                            expected_failure_list.push_back(&x);
                        }
                        else
                        {
                            failure_list.push_back(&x);
                        }
                    }
                }
            }
        }
    }

    ret = 0;
    return ret;
}

} // namespace wayround_i2p::ccutils::tst
