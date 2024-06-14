
#include <wayround_i2p/ccutils/test_suite_tool/tst.hpp>

namespace wayround_i2p::ccutils::tst
{

/*
void TSTFuncOpts::Log(
    wayround_i2p::ccutils::logger::LoggerMSGType t,
    wayround_i2p::ccutils::unicode::UString      msg
) const
{
    this->logger->Log(
        t,
        std::format(
            "[{}:{}] {}",
            this->func_info.group_name,
            this->func_info.test_name,
            msg
        )
    );
} */

void run_tests_Parameters::Log(
    wayround_i2p::ccutils::logger::LoggerMSGType t,
    wayround_i2p::ccutils::unicode::UString      msg
) const
{
    this->logger->Log(t, msg);
}

int run_tests_Parameters::AddTest(
    const TSTInfo &info
)
{
    if (std::find(
            this->group_order.begin(),
            this->group_order.end(),
            info.group_name
        )
        == this->group_order.end())
    {
        this->group_order.push_back(info.group_name);
    }

    if (this->groups.find(info.group_name) == this->groups.end())
    {
        this->groups[info.group_name] = {};
    }

    auto &grp = this->groups[info.group_name];

    if (std::find(
            grp.test_order.begin(),
            grp.test_order.end(),
            info.test_name
        )
        == grp.test_order.end())
    {
        grp.test_order.push_back(info.test_name);
    }

    if (grp.tests.find(info.test_name) == grp.tests.end())
    {
        grp.tests.insert({info.test_name, info});
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

void print_head(const run_tests_Parameters &tlo)
{
    wayround_i2p::ccutils::unicode::UString for_string = "WayRound.I2P's tool";
    wayround_i2p::ccutils::unicode::UString for_string_line;
    wayround_i2p::ccutils::unicode::UString desc_string;
    wayround_i2p::ccutils::unicode::UString uri_string;
    wayround_i2p::ccutils::unicode::UString ver_string;
    wayround_i2p::ccutils::unicode::UString mod_string;

    if (tlo.title != "")
    {
        for_string = tlo.title;
    }

    for_string = wayround_i2p::ccutils::unicode::UString("  ") + for_string;

    for (auto i = 0; i != for_string.length() + 2; i++)
    {
        for_string_line += "-";
    }

    if (tlo.description != "")
    {
        desc_string = std::format(
            "description    |   {}\n",
            tlo.description
        );
    }

    if (tlo.uri != "")
    {
        uri_string = std::format(
            "uri            |   {}\n",
            tlo.uri
        );
    }

    if (tlo.version != "")
    {
        ver_string = std::format(
            "version        |   {}\n",
            tlo.version
        );
    }

    if (tlo.mod_date != "")
    {
        mod_string = std::format(
            "modified date  |   {}\n",
            tlo.mod_date
        );
    }

    std::cout << std::format(
        R"+++({0:}
{1}
{0:}
(test suite created with WayRound.I2P's tool)

{2}{3}{4}{5})+++",
        for_string_line,
        for_string,
        desc_string,
        uri_string,
        ver_string,
        mod_string
    ) << std::endl;
}

int run_tests(int argc, char **args, const run_tests_Parameters &rtp)
{
    int ret = 1;

    unsigned int total_count = 0;

    std::vector<const TSTInfo *> success_list;
    std::vector<const TSTInfo *> failure_list;
    std::vector<const TSTInfo *> expected_failure_list;
    std::vector<const TSTInfo *> unexpected_success_list;

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

    rtp.Log(
        wayround_i2p::ccutils::logger::Status,
        std::format(
            "{} test group(s) provided",
            rtp.group_order.size()
        )
    );

    rtp.Log(wayround_i2p::ccutils::logger::Status, "");

    for (auto &group_name : rtp.group_order)
    {

        auto se03 = std::experimental::scope_exit(
            [&]()
            {
                rtp.Log(wayround_i2p::ccutils::logger::Status, "");
            }
        );

        rtp.Log(
            wayround_i2p::ccutils::logger::Status,
            std::format("next group is: {}", group_name)
        );

        if (
            auto group_itr = rtp.groups.find(
                group_name
            );
            group_itr == std::end(rtp.groups)
        )
        {
            rtp.Log(
                wayround_i2p::ccutils::logger::Error,
                std::format("'{}' not in groups", group_name)
            );
            ret = 1;
            return ret;
        }
        else
        {
            const GroupsMapItem &group = std::get<1>(*group_itr);

            std::map<std::string, std::any> iitm = {};

            rtp.Log(
                wayround_i2p::ccutils::logger::Status,
                std::format(
                    "group {} contains {} test(s)",
                    group_name,
                    group.test_order.size()
                )
            );

            rtp.Log(wayround_i2p::ccutils::logger::Status, "");

            for (auto &test_name : group.test_order)
            {

                auto se02 = std::experimental::scope_exit(
                    [&]()
                    {
                        rtp.Log(wayround_i2p::ccutils::logger::Status, "");
                    }
                );

                if (auto test_itr = group.tests.find(
                        test_name
                    );
                    test_itr == std::end(group.tests))
                {
                    rtp.Log(
                        wayround_i2p::ccutils::logger::Error,
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

                    const TSTInfo &x = std::get<1>(*test_itr);

                    LOGGER_CB_FUNCTION_TYPE cb_log_function_for_test =
                        [&rtp, &x](
                            wayround_i2p::ccutils::logger::LoggerMSGType t,
                            wayround_i2p::ccutils::unicode::UString      msg
                        ) -> void
                    {
                        rtp.logger->Log(
                            t,
                            std::format(
                                "[{}:{}] {}",
                                x.group_name,
                                x.test_name,
                                msg
                            )
                        );
                    };

                    total_count++;

                    cb_log_function_for_test(
                        wayround_i2p::ccutils::logger::Status,
                        std::format(
                            "starting: {}",
                            x.description_short
                        )
                    );

                    if (!x.func)
                    {
                        rtp.Log(
                            wayround_i2p::ccutils::logger::Error,
                            std::format(
                                "function not defined in '{}:{}'",
                                test_name,
                                group_name
                            )
                        );
                        ret = 3;
                        return ret;
                    }

                    auto res = x.func(x, iitm, cb_log_function_for_test);
                    if (res.test_success)
                    {
                        if (x.expected_failure)
                        {
                            cb_log_function_for_test(
                                wayround_i2p::ccutils::logger::UnexpectedSuccess,
                                "Test Unexpectedly Succeeded"
                            );
                            unexpected_success_list.push_back(&x);
                        }
                        else
                        {
                            cb_log_function_for_test(
                                wayround_i2p::ccutils::logger::Success,
                                "Test Succeeded"
                            );
                            success_list.push_back(&x);
                        }
                    }
                    else
                    {
                        if (x.expected_failure)
                        {
                            cb_log_function_for_test(
                                wayround_i2p::ccutils::logger::ExpectedFailure,
                                "Test Expectedly Failed"
                            );
                            expected_failure_list.push_back(&x);
                        }
                        else
                        {
                            cb_log_function_for_test(
                                wayround_i2p::ccutils::logger::Failure,
                                "Test Failed"
                            );
                            failure_list.push_back(&x);
                        }
                        rtp.Log(
                            wayround_i2p::ccutils::logger::Status,
                            ""
                        );
                    }
                }
            }
        }
    }

    if (failure_list.size() == 0)
    {
        rtp.Log(
            wayround_i2p::ccutils::logger::Success,
            "all tests ok"
        );
        ret = 0;
    }
    else
    {
        rtp.Log(
            wayround_i2p::ccutils::logger::Failure, "errors found"
        );
        ret = 5;
    }
    return ret;
}

} // namespace wayround_i2p::ccutils::tst
