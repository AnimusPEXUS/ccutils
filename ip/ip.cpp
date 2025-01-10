
#include <wayround_i2p/ccutils/ip/ip.hpp>

namespace wayround_i2p::ccutils::ip
{

inline bool check_val_fits_1_byte(long long val)
{
    return (val >= 0 && val <= 0xff);
}

inline bool check_val_fits_2_bytes(long long val)
{
    return (val >= 0 && val <= 0xffff);
}

regexp::Pattern_shared PORT_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newExactChar(':'),
               regexp::Pattern::newCharIsDigit()
                   ->setName("number")
                   ->setMinCount(1)
                   ->unsetMaxCount()
              }
        )
              ->setName("PORT_STR_PATTERN");
    return ret;
}

regexp::Pattern_shared CIDR_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newExactChar('/'),
               regexp::Pattern::newCharIsDigit()
                   ->setName("number")
                   ->setMinCount(1)
                   ->unsetMaxCount()
              }
        )
              ->setName("CIDR_STR_PATTERN");
    return ret;
}

regexp::Pattern_shared IPv4_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("1"),
               regexp::Pattern::newExactChar('.')
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("2"),
               regexp::Pattern::newExactChar('.')
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("3"),
               regexp::Pattern::newExactChar('.')
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("4")
              }
        )
              ->setName("IPv4_STR_PATTERN");
    return ret;
}

regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newGroup(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(1, 4)
                        ->setName("number"),
                    regexp::Pattern::newExactChar(':')
                        ->setMinMaxCount(1, 1)
                   }
               )
                   ->setMinMaxCount(7, 7)
                   ->setName("numbers"),
               regexp::Pattern::newCharIsXDigit()
                   ->setMinMaxCount(1, 4)
                   ->setName("8")
              }

        )
              ->setName("IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN");
    return ret;
}

regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newGroup(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(1, 4)
                        ->setName("number"),
                    regexp::Pattern::newExactChar(':')
                        ->setMinMaxCount(1, 1)
                   }
               )
                   ->setMinMaxCount(6, 6)
                   ->setName("numbers"),
               IPv4_STR_PATTERN()
              }

        )
              ->setName("IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4");
    return ret;
}

regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newOrGroup(
                   {regexp::Pattern::newGroup(
                        {regexp::Pattern::newCharIsXDigit()
                             ->setMinMaxCount(1, 4)
                             ->setName("number"),
                         regexp::Pattern::newExactChar(':')
                             ->setExactCount(1)
                        }
                    )
                        ->setMinCount(1)
                        ->unsetMaxCount(),
                    regexp::Pattern::newExactChar(':')
                        ->setExactCount(1)
                   }
               )
                   ->setName("first half"),
               regexp::Pattern::newOrGroup(
                   {regexp::Pattern::newGroup(
                        {regexp::Pattern::newExactChar(':')
                             ->setExactCount(1),
                         regexp::Pattern::newCharIsXDigit()
                             ->setMinMaxCount(1, 4)
                             ->setName("number")
                        }
                    )
                        ->setMinCount(1)
                        ->unsetMaxCount(),
                    regexp::Pattern::newExactChar(':')
                        ->setExactCount(1)
                   }
               )
                   ->setName("second half")
              }
        )
              ->setName("IPv6_SHORT_GRP_HEX_STR_PATTERN");

    return ret;
}

regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newGroup(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(0, 4)
                        ->setName("number"),
                    regexp::Pattern::newExactChar(':')
                        ->setMinCount(1)
                        ->setMaxCount(2)
                   }
               )
                   ->setMinCount(1)
                   ->setMaxCount(6),
               IPv4_STR_PATTERN()
              }
        )
              ->setName("IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4");
    return ret;
}

regexp::Pattern_shared IPv6_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newExactChar('[')
                   ->setMaxCount(1)
                   ->unsetMinCount(),
               regexp::Pattern::newOrGroup(
                   {/* long 2 byte */
                    IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN(),
                    IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4(),
                    /* short */
                    IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4(),
                    IPv6_SHORT_GRP_HEX_STR_PATTERN()
                   }
               )
                   ->setName("IPv6_STR_PATTERN_OR_GROUP"),
               regexp::Pattern::newExactChar(']')
                   ->setMaxCount(1)
                   ->unsetMinCount()
              }
        )
              ->setName("IPv6_STR_PATTERN");
    return ret;
}

regexp::Pattern_shared IP_STR_PATTERN()
{
    regexp::Pattern_shared ret
        = regexp::Pattern::newOrGroup(
              {IPv4_STR_PATTERN(),
               IPv6_STR_PATTERN()
              }
        )
              ->setExactCount(1)
              ->setName("IP_STR_PATTERN");
    return ret;
}

regexp::Pattern_shared IP_AND_CIDR_OR_PORT_STR_PATTERN(
    IP_AND_CIDR_OR_PORT_STR_PATTERN_mode mode
)
{
    regexp::Pattern_shared ret;
    regexp::Pattern_shared port_or_cidr;

    if (mode == IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_only)
    {
        ret = IP_STR_PATTERN()->setName("ip");
    }
    else
    {
        switch (mode)
        {
            default:
                throw wayround_i2p::ccutils::errors::New(
                    "invalid mode",
                    __FILE__,
                    __LINE__
                );

            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_cidr_or_port:
                port_or_cidr
                    = regexp::Pattern::newOrGroup(
                          {PORT_STR_PATTERN(),
                           CIDR_STR_PATTERN()
                          }
                    )
                          ->setExactCount(1);
                break;
            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_cidr:
                port_or_cidr = CIDR_STR_PATTERN()->setExactCount(1);
                break;
            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_port:
                port_or_cidr = PORT_STR_PATTERN()->setExactCount(1);
                break;

            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_cidr_or_port:
                port_or_cidr
                    = regexp::Pattern::newOrGroup(
                          {PORT_STR_PATTERN(),
                           CIDR_STR_PATTERN()
                          }
                    )
                          ->unsetMinCount()
                          ->setMaxCount(1);
                break;
            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_cidr:
                port_or_cidr = CIDR_STR_PATTERN()->unsetMinCount()->setMaxCount(1);
                break;
            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_port:
                port_or_cidr = PORT_STR_PATTERN()->unsetMinCount()->setMaxCount(1);
                break;
        }

        ret = regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setName("ip"),
             port_or_cidr
                 ->setName("port_or_cidr")
            }
        );
    }

    return ret;
}

error_ptr
    getValueFrom_PORT_STR_PATTERN_Result(
        bool                        port_1_cidr_0,
        const regexp::Result_shared res,
        bool                       &has_value,
        std::uint16_t              &value
    )
{
    return getValueFrom_port_or_cidr_STR_PATTERN_Result(
        true,
        res,
        has_value,
        value
    );
}

error_ptr
    getValueFrom_CIDR_STR_PATTERN_Result(
        bool                        port_1_cidr_0,
        const regexp::Result_shared res,
        bool                       &has_value,
        std::uint16_t              &value
    )
{
    return getValueFrom_port_or_cidr_STR_PATTERN_Result(
        false,
        res,
        has_value,
        value
    );
}

error_ptr
    getValueFrom_port_or_cidr_STR_PATTERN_Result(
        bool                        port_1_cidr_0,
        const regexp::Result_shared res,
        bool                       &has_value,
        std::uint16_t              &value
    )
{
    auto err = regexp::ResultRoutineCheck<true, true>(
        res,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    auto res_VALUE_STR_PATTERN = res->findByNameRec(
        (port_1_cidr_0 ? "PORT_STR_PATTERN" : "CIDR_STR_PATTERN")
    );

    err = regexp::ResultRoutineCheck<true, true>(
        res_VALUE_STR_PATTERN,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    auto res_number = res_VALUE_STR_PATTERN->findByNameRec("number");

    err = regexp::ResultRoutineCheck<true, true>(
        res_number,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    auto res_number_str = res_number->getMatchedString();

    long long x = 0;

    try
    {
        x = std::stoll(res_number_str);
    }
    catch (const std::exception &e)
    {
        return wayround_i2p::ccutils::errors::New(
            std::format("std::stoul error: {}", e.what()),
            __FILE__,
            __LINE__
        );
    }

    if (!check_val_fits_2_bytes(x))
    {
        return wayround_i2p::ccutils::errors::New(
            "!res - value too large",
            __FILE__,
            __LINE__
        );
    }

    value = x;

    return nullptr;
}

error_ptr getValuesFrom_IPv4_STR_PATTERN_Result(
    const regexp::Result_shared  res,
    std::array<std::uint8_t, 4> &ipv4
)
{
    // todo: optimizations needed? function code is messy!
    auto err = regexp::ResultRoutineCheck<true, true>(
        res,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    if (!res->matched)
    {
        return wayround_i2p::ccutils::errors::New(
            "dismatch",
            __FILE__,
            __LINE__
        );
    }

    auto res3 = res->findByName("IPv4_STR_PATTERN");
    err       = regexp::ResultRoutineCheck<true, true>(
        res3,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    if (!res3->matched)
    {
        return wayround_i2p::ccutils::errors::New(
            "dismatch",
            __FILE__,
            __LINE__
        );
    }

    for (unsigned char i = 0; i < 4; i++)
    {
        auto res2 = res3->findByNameRec(std::to_string(i + 1));

        err = regexp::ResultRoutineCheck<true, true>(
            res2,
            __FILE__,
            __LINE__
        );
        if (err)
        {
            return err;
        }

        auto x = res2->getMatchedString();
        try
        {
            ipv4[3 - i] = std::stoul(x);
        }
        catch (std::invalid_argument const &ex)
        {
            return wayround_i2p::ccutils::errors::New(
                "std::stoi : invalid_argument",
                __FILE__,
                __LINE__
            );
        }
        catch (std::out_of_range const &ex)
        {
            return wayround_i2p::ccutils::errors::New(
                "std::stoi : out_of_range",
                __FILE__,
                __LINE__
            );
        }
    }

    // todo: add cidr and port retrieval

    return nullptr;
}

error_ptr getValuesFrom_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    std::array<std::uint16_t, 8> &ipv6,
    bool                         &ipv6_short,
    bool                         &ipv6_v4_comb
)
{
    auto err = regexp::ResultRoutineCheck<true, true>(
        res,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    if (!res->matched)
    {
        return wayround_i2p::ccutils::errors::New(
            "dismatch",
            __FILE__,
            __LINE__
        );
    }

    // return nullptr; // ok

    auto res_or_group = res->findByNameRec("IPv6_STR_PATTERN_OR_GROUP");

    err = regexp::ResultRoutineCheck<true, true>(
        res_or_group,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    if (!res_or_group->matched)
    {
        return wayround_i2p::ccutils::errors::New(
            "dismatch",
            __FILE__,
            __LINE__
        );
    }

    // return nullptr; // ok

    auto ms = res_or_group->getMatchedString();

    // return nullptr; // ok

    std::deque<UString> ms_spl;
    ms.split(ms_spl, ":");

    // return nullptr; // ok

    ipv6_v4_comb = false;

    auto res_comb = res->findByNameRec("IPv4_STR_PATTERN");
    if (res_comb)
    {
        ipv6_v4_comb = true;
    }

    // return nullptr; // ok

    if (ipv6_v4_comb)
    {
        union
        {
            std::array<std::uint16_t, 2> ipv6_ints;
            std::array<std::uint8_t, 4>  ipv4_bytes;
        };

        err = getValuesFrom_IPv4_STR_PATTERN_Result(res_comb, ipv4_bytes);
        if (err)
        {
            return err;
        }

        ipv6[0] = ipv6_ints[0];
        ipv6[1] = ipv6_ints[1];
    }

    // return nullptr; // ok

    ipv6_short = false;

    decltype(ms_spl)::iterator short_index_itr;

    // determine if input string is shortened ipv6
    for (
        unsigned char short_index = (ipv6_v4_comb ? 1 : 0);
        short_index < (ipv6_v4_comb ? 7 : 8);
        short_index++
    )
    {
        if (ms_spl[short_index] == "")
        {
            ipv6_short      = true;
            short_index_itr = ms_spl.begin() + short_index;
            break;
        }
    }

    // return nullptr; // ok

    if (ipv6_short)
    {
        while (ms_spl.size() != (ipv6_v4_comb ? 7 : 8))
        {
            short_index_itr = ms_spl.insert(short_index_itr, "");
        }
    }

    // return nullptr; // ok

    if (ms_spl.size() != (ipv6_v4_comb ? 7 : 8))
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid size of ms_spl",
            __FILE__,
            __LINE__
        );
    }

    decltype(ms_spl)::iterator index_itr;

    for (
        unsigned char i = (ipv6_v4_comb ? 1 : 0);
        i < (ipv6_v4_comb ? 7 : 8);
        i++
    )
    {
        index_itr = ms_spl.end() - (i + 1);

        UString val_str = *index_itr;
        if (val_str == "")
        {
            val_str = "0";
        }

        std::uint16_t val_int = 0;

        try
        {
            val_int = std::stoul(val_str.to_string(), nullptr, 16);
        }
        catch (const std::exception &e)
        {
            return wayround_i2p::ccutils::errors::New(
                std::format("std::stoul error: {}", e.what()),
                __FILE__,
                __LINE__
            );
        }

        ipv6[(ipv6_v4_comb ? 2 + (i - 1) : i)] = val_int;
    }

    return nullptr;
}

} // namespace wayround_i2p::ccutils::ip
