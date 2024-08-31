#include <csignal>

#include <wayround_i2p/ccutils/ip/ip.hpp>

namespace regexp = wayround_i2p::ccutils::regexp;
namespace ip     = wayround_i2p::ccutils::ip;

using UString = wayround_i2p::ccutils::unicode::UString;

const std::vector<UString> testing_examples{
    "0.0.0.0",
    "12.34.56.78",
    "0.0.0.0/24",
    "12.34.56.78/24",
    "0.0.0.0:9050",
    "12.34.56.78:9050",
    "[::]:8080",
    "0123:4567:89ab:cdef:0123:4567:89ab:cdf0",
    "0123:4567:89ab::cdef",
    "[0123:4567:89ab::cdef]:9050",
    "::ffff:192.0.2.128",
    "2345:0425:2CA1:0000:0000:0567:5673:23b5",
    "2345:0425:2CA1::0567:5673:23b5",
    "2266:25::12:0:ad12"

};

wayround_i2p::ccutils::tst::TSTFuncResult main_001(
    const wayround_i2p::ccutils::tst::TSTInfo    &func_info,
    std::map<std::string, std::any>              &iitm,
    wayround_i2p::ccutils::logger::LoggerI_shared logger
)
{

    for (const auto &i : testing_examples)
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
                {ip::IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN, "IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN", false},
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
