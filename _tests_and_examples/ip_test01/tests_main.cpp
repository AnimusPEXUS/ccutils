#include <algorithm>
// #include <csignal>

#include <wayround_i2p/ccutils/ip/ip.hpp>

namespace regexp = wayround_i2p::ccutils::regexp;
namespace ip     = wayround_i2p::ccutils::ip;

using UString = wayround_i2p::ccutils::unicode::UString;

void uniqsort(std::vector<UString> &x)
{
    // note: for some reason unique doesn't work with std::string properly
    // auto y = std::unique(x.begin(), x.end());
    // x.resize(std::distance(x.begin(), y));

    {
        std::vector<UString> new_vec;

        for (const auto &x_i : x)
        {
            for (const auto &new_vec_i : new_vec)
            {
                if (new_vec_i == x_i)
                {
                    goto cont;
                }
            }
            new_vec.push_back(x_i);
        cont:
        }
        x = new_vec;
    }

    std::sort(x.begin(), x.end());
}

const std::vector<UString> testing_ipv4 = []()
{
    auto x = std::vector<UString>{
        "0.0.0.0",
        "12.34.56.78"
    };

    uniqsort(x);
    return x;
}();

const std::vector<UString> testing_ipv6 = []()
{
    auto x = std::vector<UString>{
        "0123:4567:89ab:cdef:0123:4567:89ab:cdf0",
        "0123:4567:89ab::cdef",
        "2345:0425:2CA1:0000:0000:0567:5673:23b5",
        "2266:25::12:0:ad12",
        "2345:0425:2CA1::0567:5673:23b5",
        "::12:0:ad12",
        "2266:25::",
        "::",
        "::ffff:192.0.2.128"
    };

    uniqsort(x);
    return x;
}();

const std::vector<UString> testing_cidr = []()
{
    auto x = std::vector<UString>{
        "24",
    };

    uniqsort(x);
    return x;
}();

const std::vector<UString> testing_port = []()
{
    auto x = std::vector<UString>{
        "9050",
    };

    uniqsort(x);
    return x;
}();

const std::vector<UString> testing_examples_ipv4 = []()
{
    std::vector<UString> ret;
    for (const auto &i : testing_ipv4)
    {
        ret.push_back(i);
    }

    for (const auto &i : testing_ipv4)
    {
        for (const auto &j : testing_cidr)
        {
            ret.push_back(std::format("{}/{}", i, j));
        }
    }

    for (const auto &i : testing_ipv4)
    {
        for (const auto &j : testing_port)
        {
            ret.push_back(std::format("{}:{}", i, j));
        }
    }
    return ret;
}();

const std::vector<UString> testing_examples_ipv6 = []()
{
    std::vector<UString> ret;
    for (const auto &i : testing_ipv6)
    {
        ret.push_back(i);
    }

    for (const auto &i : testing_ipv6)
    {
        for (const auto &j : testing_cidr)
        {
            ret.push_back(std::format("{}/{}", i, j));
        }
    }

    for (const auto &i : testing_ipv6)
    {
        for (const auto &j : testing_port)
        {
            ret.push_back(std::format("[{}]:{}", i, j));
        }
    }
    return ret;
}();

const std::vector<UString> testing_examples_all = []()
{
    std::vector<UString> ret;

    for (const auto &i : {
             testing_examples_ipv4,
             testing_examples_ipv6,
         })
    {
        for (const auto &j : i)
        {
            ret.push_back(j);
        }
    }

    for (const auto &j : testing_port)
    {
        ret.push_back(std::format(":{}", j));
    }

    for (const auto &j : testing_cidr)
    {
        ret.push_back(std::format("/{}", j));
    }

    return ret;
}();

// -----------------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_print_examples(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    logger->LogSplitLines(
        wayround_i2p::ccutils::logger::Status,
        "here are sample IPs used in tests below. \n"
        "can be used to quickly navigate to they'r parsing logs."
    );

    for (const auto &i : testing_examples_all)
    {
        logger->Log(
            wayround_i2p::ccutils::logger::Status,
            i
        );
    }
    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_print_examples_i = {
    .group_name = "main",
    .test_name  = "print_examples",
    .func       = main_print_examples
};

// -----------------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_regexps_tests(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{

    for (const auto &i : testing_examples_all)
    {
        logger->Log(
            wayround_i2p::ccutils::logger::Status,
            std::format(" test subject {}", i)
        );

        for (
            auto &j :
            std::vector<
                std::tuple<
                    std::function<regexp::Pattern_shared()>, // get regexp to test
                    UString,                                 // title
                    bool                                     // call debugger
                    >>{
                {ip::IPv4_STR_PATTERN,                              "IPv4_STR_PATTERN",                              false},
                {ip::IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN,           "IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN",           false},
                {ip::IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4, "IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4", false},
                {ip::IPv6_SHORT_GRP_HEX_STR_PATTERN,                "IPv6_SHORT_GRP_HEX_STR_PATTERN",                false},
                {ip::IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4,      "IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4",      false},
                {ip::IPv6_STR_PATTERN,                              "IPv6_STR_PATTERN",                              false},
                {ip::IP_STR_PATTERN,                                "IP_STR_PATTERN",                                false},

                {ip::IP_AND_MUST_PORT_OR_CIDR_PATTERN,              "IP_AND_MUST_PORT_OR_CIDR_PATTERN",              false},
                {ip::IP_AND_MUST_PORT_PATTERN,                      "IP_AND_MUST_PORT_PATTERN",                      false},
                {ip::IP_AND_MUST_CIDR_PATTERN,                      "IP_AND_MUST_CIDR_PATTERN",                      false},

                {ip::IP_AND_OPT_PORT_OR_CIDR_PATTERN,               "IP_AND_OPT_PORT_OR_CIDR_PATTERN",               false},
                {ip::IP_AND_OPT_PORT_PATTERN,                       "IP_AND_OPT_PORT_PATTERN",                       false},
                {ip::IP_AND_OPT_CIDR_PATTERN,                       "IP_AND_OPT_CIDR_PATTERN",                       false},

                {ip::OPT_IP_AND_MUST_PORT_OR_CIDR_PATTERN,          "OPT_IP_AND_MUST_PORT_OR_CIDR_PATTERN",          false},
                {ip::OPT_IP_AND_MUST_PORT_PATTERN,                  "OPT_IP_AND_MUST_PORT_PATTERN",                  false},
                {ip::OPT_IP_AND_MUST_CIDR_PATTERN,                  "OPT_IP_AND_MUST_CIDR_PATTERN",                  false},

                {ip::OPT_IP_AND_OPT_PORT_OR_CIDR_PATTERN,           "OPT_IP_AND_OPT_PORT_OR_CIDR_PATTERN",           false},
                {ip::OPT_IP_AND_OPT_PORT_PATTERN,                   "OPT_IP_AND_OPT_PORT_PATTERN",                   false},
                {ip::OPT_IP_AND_OPT_CIDR_PATTERN,                   "OPT_IP_AND_OPT_CIDR_PATTERN",                   false}
        }
        )
        {
            logger->Log(
                wayround_i2p::ccutils::logger::Status,
                std::format(" {}", std::get<1>(j))
            );

            /*
                if (std::get<2>(j))
                {
                    std::raise(SIGUSR1);
                }
            */

            auto res = (std::get<0>(j))()->match(
                std::shared_ptr<UString>(new UString(i))
            );

            if (res->error)
            {
                logger->Log(
                    wayround_i2p::ccutils::logger::Error,
                    res->error->Error()
                );
                continue;
            }

            logger->Log(
                wayround_i2p::ccutils::logger::Status,
                std::format("  {}", res->matched ? "matched" : "dismatched")
            );

            logger->LogSplitLines(
                wayround_i2p::ccutils::logger::Status,
                res->repr_as_text(false)
            );
        }

        logger->LogSplitLines(
            wayround_i2p::ccutils::logger::Status,
            "-//--//--//--//--//--//--//--//--//--//--//--//--//--//-\n"
            "//--//--//--//--//--//--//--//--//--//--//--//--//--//--\n"
            "/--//--//--//--//--//--//--//--//--//--//--//--//--//--/\n"
            "--//--//--//--//--//--//--//--//--//--//--//--//--//--//\n"
        );
    }

    return {false};
}

wayround_i2p::ccutils::tst::TSTInfo main_regexps_tests_i = {
    .group_name = "main",
    .test_name  = "regexps_tests",
    .func       = main_regexps_tests
};

// -----------------------------------------------------------------

/*
wayround_i2p::ccutils::tst::TSTFuncResult main_regexps_shortcuts_tests(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{

    for (const auto &i : testing_examples_all)
    {
        logger->Log(
            wayround_i2p::ccutils::logger::Status,
            std::format(" test subject {}", i)
        );

        auto res = ip::IP_STR_PATTERN()->match(i);

        if (!res)
        {
            logger->Log(
                wayround_i2p::ccutils::logger::Error,
                "No match"
            );
            continue;
        }

        if (res->error)
        {
            logger->Log(
                wayround_i2p::ccutils::logger::Error,
                std::format("match error: {}", res->error->Error())
            );
            continue;
        }

        for (
            const auto &j :
            std::vector<UString>{
                "IP_STR_PATTERN",
                "IPv4_STR_PATTERN",
                "IPv6_STR_PATTERN",
                "IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN",
                "IPv6_SHORT_GRP_HEX_STR_PATTERN",
            }
        )
        {

            logger->Log(
                wayround_i2p::ccutils::logger::Status,
                std::format(" trying shortcut {}", j)
            );

            auto res2 = res->getShortcutResult(j);

            if (!res2)
            {
                logger->Log(
                    wayround_i2p::ccutils::logger::Error,
                    "No match"
                );
                continue;
            }

            if (res2->error)
            {
                logger->Log(
                    wayround_i2p::ccutils::logger::Error,
                    std::format("match error: {}", res->error->Error())
                );
                continue;
            }

            logger->Log(
                wayround_i2p::ccutils::logger::Status,
                std::format("  {}", res2->matched ? "matched" : "dismatched")
            );

            logger->LogSplitLines(
                wayround_i2p::ccutils::logger::Status,
                res2->repr_as_text(false)
            );
        }

        logger->Log(
            wayround_i2p::ccutils::logger::Status,
            "-----------------------------"
        );
    }

    return {false};
}

wayround_i2p::ccutils::tst::TSTInfo main_regexps_shortcuts_tests_i = {
    .group_name = "main",
    .test_name  = "regexps_shortcuts_tests",
    .func       = main_regexps_shortcuts_tests
};
*/

// -----------------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_IP_create(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{
    for (const auto &i : testing_examples_all)
    {
        logger->Log(
            wayround_i2p::ccutils::logger::Status,
            std::format("subject: {}", i)
        );

        auto res = wayround_i2p::ccutils::ip::IP::create();
        auto err = res->setAllFromString(i);

        if (err)
        {
            logger->Log(
                wayround_i2p::ccutils::logger::Failure,
                std::format("  - couldn't parse: {}", err->Error())
            );
            continue;
        }

        logger->LogSplitLines(
            wayround_i2p::ccutils::logger::Status,
            std::format(
                "   IP object contents: {}",
                res->debugRepr()
            )
        );

        logger->LogSplitLines(
            wayround_i2p::ccutils::logger::Status,
            std::format(
                "   IP outpat results:\n"
                "      getAllAsString       : {}\n"
                "      getIPv4AsString      : {}\n"
                "      getIPv6AsStringLong  : {}\n"
                "      getIPv6AsStringShort : {}\n"
                "      getPortString        : {}\n"
                "      getCIDRString        : {}\n",
                res->getAllAsString(),
                (res->hasIPv4() ? res->getIPv4AsString() : "-"),
                (res->hasIPv6() ? res->getIPv6AsStringLong() : "-"),
                (res->hasIPv6() ? res->getIPv6AsStringShort() : "-"),
                (res->hasPort() ? res->getPortString() : "-"),
                (res->hasCIDR() ? res->getCIDRString() : "-")
            )
        );
    }

    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_IP_create_i = {
    .group_name = "main",
    .test_name  = "IP_create",
    .func       = main_IP_create
};
