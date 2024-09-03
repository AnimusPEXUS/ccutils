#include <algorithm>
#include <csignal>

#include <wayround_i2p/ccutils/ip/ip.hpp>

namespace regexp = wayround_i2p::ccutils::regexp;
namespace ip     = wayround_i2p::ccutils::ip;

using UString = wayround_i2p::ccutils::unicode::UString;

void uniqsort(std::vector<UString> &x)
{
    auto y = std::unique(x.begin(), x.end());
    x.erase(y, x.end());
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
        "::ffff:192.0.2.128",
        "2345:0425:2CA1:0000:0000:0567:5673:23b5",
        "2345:0425:2CA1::0567:5673:23b5",
        "2266:25::12:0:ad12"
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
        ":9050",
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
             testing_examples_ipv6
         })
    {
        std::merge(
            ret.begin(),
            ret.end(),
            i.begin(),
            i.end(),
            std::back_inserter(ret)
        );
    }

    return ret;
}();

// -----------------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_000(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{

    for (const auto &i : testing_examples_all)
    {
        logger->Log(
            wayround_i2p::ccutils::logger::Status,
            i
        );
    }
    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_000_i = {
    .group_name        = "main",
    .test_name         = "000",
    .description_short = "testing example list",
    .func              = main_000
};

// -----------------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
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
                {ip::IP_STR_PATTERN, "IP_STR_PATTERN", false},
                {ip::IPv4_STR_PATTERN, "IPv4_STR_PATTERN", false},
                {ip::IPv6_STR_PATTERN, "IPv6_STR_PATTERN", false},
                {ip::IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN, "IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN", false},
                {ip::IPv6_SHORT_GRP_HEX_STR_PATTERN, "IPv6_SHORT_GRP_HEX_STR_PATTERN", false},
                {std::bind(ip::IP_AND_CIDR_OR_PORT_STR_PATTERN, ip::IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_cidr_or_port), "IP_AND_CIDR_OR_PORT_STR_PATTERN(ip_and_must_cidr_or_port)", false},
                {std::bind(ip::IP_AND_CIDR_OR_PORT_STR_PATTERN, ip::IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_cidr), "IP_AND_CIDR_OR_PORT_STR_PATTERN(ip_and_must_cidr)", false},
                {std::bind(ip::IP_AND_CIDR_OR_PORT_STR_PATTERN, ip::IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_port), "IP_AND_CIDR_OR_PORT_STR_PATTERN(ip_and_must_port)", false},
                {std::bind(ip::IP_AND_CIDR_OR_PORT_STR_PATTERN, ip::IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_cidr_or_port), "IP_AND_CIDR_OR_PORT_STR_PATTERN(ip_and_opt_cidr_or_port)", false},
                {std::bind(ip::IP_AND_CIDR_OR_PORT_STR_PATTERN, ip::IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_cidr), "IP_AND_CIDR_OR_PORT_STR_PATTERN(ip_and_opt_cidr)", false},
                {std::bind(ip::IP_AND_CIDR_OR_PORT_STR_PATTERN, ip::IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_port), "IP_AND_CIDR_OR_PORT_STR_PATTERN(ip_and_opt_port)", false},
                {std::bind(ip::IP_AND_CIDR_OR_PORT_STR_PATTERN, ip::IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_only), "IP_AND_CIDR_OR_PORT_STR_PATTERN(ip_only)", false}
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

            auto res = (std::get<0>(j))()->match(i);

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
                res->repr_as_text({true})
            );
        }

        logger->Log(
            wayround_i2p::ccutils::logger::Status,
            "-----------------------------"
        );
    }

    return {false};
}

wayround_i2p::ccutils::tst::TSTInfo main_001_i = {
    .group_name        = "main",
    .test_name         = "001",
    .description_short = "testing ip strings parsing",
    .func              = main_001
};

// -----------------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_002(
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
        auto ip_res = wayround_i2p::ccutils::ip::IPv4::createFromString(i);
        auto res    = std::get<0>(ip_res);
        auto err    = std::get<1>(ip_res);
        if (err != nullptr)
        {
            logger->Log(
                wayround_i2p::ccutils::logger::Failure,
                std::format("  - couldn't parse: {}", err->Error())
            );
            continue;
        }

        logger->Log(
            wayround_i2p::ccutils::logger::Success,
            std::format("  + parsed: {}", res->toString())
        );
    }

    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_002_i = {
    .group_name        = "main",
    .test_name         = "002",
    .description_short = "testing IPv4::createFromString()",
    .func              = main_002
};

// -----------------------------------------------------------------

wayround_i2p::ccutils::tst::TSTFuncResult main_003(
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
        auto ip_res = wayround_i2p::ccutils::ip::IPv6::createFromString(i);
        auto res    = std::get<0>(ip_res);
        auto err    = std::get<1>(ip_res);
        if (err != nullptr)
        {
            logger->Log(
                wayround_i2p::ccutils::logger::Failure,
                std::format("  - couldn't parse: {}", err->Error())
            );
            continue;
        }

        logger->LogSplitLines(
            wayround_i2p::ccutils::logger::Success,
            std::format(
                R"---(  + parsed: 
      short {}
      long  {}
)---",
                res->toStringShort(),
                res->toStringLong()
            )
        );
    }

    return {true};
}

wayround_i2p::ccutils::tst::TSTInfo main_003_i = {
    .group_name        = "main",
    .test_name         = "003",
    .description_short = "testing IPv6::createFromString()",
    .func              = main_003
};
