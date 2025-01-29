
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
    auto ret
        = regexp::Pattern::newOrGroup(
              {IPv4_STR_PATTERN(),
               IPv6_STR_PATTERN()
              }
        )
              ->setName("IP_STR_PATTERN");
    return ret;
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

regexp::Pattern_shared PORT_OR_CIDR_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newOrGroup(
              {PORT_STR_PATTERN(),
               CIDR_STR_PATTERN()}
        )
              ->setName("PORT_OR_CIDR_STR_PATTERN");
    return ret;
}

regexp::Pattern_shared IP_ONLY_PATTERN()
{
    return regexp::makeExact(IP_STR_PATTERN());
}

regexp::Pattern_shared PORT_ONLY_PATTERN()
{
    return regexp::makeExact(PORT_STR_PATTERN());
}

regexp::Pattern_shared CIDR_ONLY_PATTERN()
{
    return regexp::makeExact(CIDR_STR_PATTERN());
}

regexp::Pattern_shared PORT_OR_CIDR_ONLY_PATTERN()
{
    return regexp::makeExact(PORT_OR_CIDR_STR_PATTERN());
}

regexp::Pattern_shared IP_AND_MUST_PORT_OR_CIDR_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setExactCount(1),
             PORT_OR_CIDR_STR_PATTERN()
                 ->setExactCount(1)
            }
        )
    );
}

regexp::Pattern_shared IP_AND_MUST_PORT_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setExactCount(1),
             PORT_STR_PATTERN()
                 ->setExactCount(1)
            }
        )
    );
}

regexp::Pattern_shared IP_AND_MUST_CIDR_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setExactCount(1),
             CIDR_STR_PATTERN()
                 ->setExactCount(1)
            }
        )
    );
}

regexp::Pattern_shared IP_AND_OPT_PORT_OR_CIDR_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setExactCount(1),
             PORT_OR_CIDR_STR_PATTERN()
                 ->setMinMaxCount(0, 1)
            }
        )
    );
}

regexp::Pattern_shared IP_AND_OPT_PORT_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setExactCount(1),
             PORT_STR_PATTERN()
                 ->setMinMaxCount(0, 1)
            }
        )
    );
}

regexp::Pattern_shared IP_AND_OPT_CIDR_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setExactCount(1),
             PORT_OR_CIDR_STR_PATTERN()
                 ->setMinMaxCount(0, 1)
            }
        )
    );
}

regexp::Pattern_shared OPT_IP_AND_MUST_PORT_OR_CIDR_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setMinMaxCount(0, 1),
             PORT_OR_CIDR_STR_PATTERN()
                 ->setExactCount(1)
            }
        )
    );
}

regexp::Pattern_shared OPT_IP_AND_MUST_PORT_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setMinMaxCount(0, 1),
             PORT_STR_PATTERN()
                 ->setExactCount(1)
            }
        )
    );
}

regexp::Pattern_shared OPT_IP_AND_MUST_CIDR_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setMinMaxCount(0, 1),
             CIDR_STR_PATTERN()
                 ->setExactCount(1)
            }
        )
    );
}

regexp::Pattern_shared OPT_IP_AND_OPT_PORT_OR_CIDR_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setMinMaxCount(0, 1),
             PORT_OR_CIDR_STR_PATTERN()
                 ->setMinMaxCount(0, 1)
            }
        )
    );
}

regexp::Pattern_shared OPT_IP_AND_OPT_PORT_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setMinMaxCount(0, 1),
             PORT_STR_PATTERN()
                 ->setMinMaxCount(0, 1)
            }
        )
    );
}

regexp::Pattern_shared OPT_IP_AND_OPT_CIDR_PATTERN()
{
    return regexp::makeExact(
        regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setMinMaxCount(0, 1),
             CIDR_STR_PATTERN()
                 ->setMinMaxCount(0, 1)
            }
        )
    );
}

error_ptr
    getValueFrom_PORT_STR_PATTERN_Result(
        const regexp::Result_shared res,
        bool                       &matched,
        std::uint16_t              &value
    )
{
    return getValueFrom_port_or_cidr_STR_PATTERN_Result(
        true,
        res,
        matched,
        value
    );
}

error_ptr
    getValueFrom_CIDR_STR_PATTERN_Result(
        const regexp::Result_shared res,
        bool                       &matched,
        std::uint16_t              &value
    )
{
    return getValueFrom_port_or_cidr_STR_PATTERN_Result(
        false,
        res,
        matched,
        value
    );
}

error_ptr
    getValueFrom_port_or_cidr_STR_PATTERN_Result(
        bool                        port_1_cidr_0,
        const regexp::Result_shared res,
        bool                       &matched,
        std::uint16_t              &value
    )
{
    matched = false;

    auto err = regexp::ResultRoutineCheck(
        res,
        true,
        false,
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

    if (!res_VALUE_STR_PATTERN)
    {
        return nullptr;
    }

    if (res_VALUE_STR_PATTERN->error)
    {
        return res_VALUE_STR_PATTERN->error;
    }

    if (!res_VALUE_STR_PATTERN->matched)
    {
        return nullptr;
    }

    auto res_number = res_VALUE_STR_PATTERN->findByNameRec("number");

    if (!res_number)
    {
        return nullptr;
    }

    if (res_number->error)
    {
        return res_number->error;
    }

    if (!res_number->matched)
    {
        return nullptr;
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

    matched = true;

    return nullptr;
}

error_ptr getValuesFrom_IPv4_STR_PATTERN_Result(
    const regexp::Result_shared  res,
    bool                        &matched,
    std::array<std::uint8_t, 4> &ipv4
)
{
    matched  = false;
    // todo: optimizations needed? function code is messy!
    auto err = regexp::ResultRoutineCheck(
        res,
        true,
        false,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    auto res3 = res->findByNameRec("IPv4_STR_PATTERN");

    if (!res3)
    {
        return nullptr;
    }

    if (res3->error)
    {
        return res3->error;
    }

    if (!res3->matched)
    {
        return nullptr;
    }

    for (unsigned char i = 0; i < 4; i++)
    {
        auto res2 = res3->findByNameRec(std::to_string(i + 1));

        if (!res2)
        {
            return nullptr;
        }

        if (res2->error)
        {
            return res2->error;
        }

        if (!res2->matched)
        {
            return nullptr;
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

    matched = true;

    return nullptr;
}

error_ptr getValuesFrom_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    bool                         &matched,
    std::array<std::uint16_t, 8> &ipv6,
    std::array<std::uint8_t, 4>  &ipv4,
    bool                         &ipv6_short,
    bool                         &ipv6_v4_comb,
    bool                          ret_bigendian
)
{

    matched = false;

    auto err = regexp::ResultRoutineCheck(
        res,
        true,
        false,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    auto res_or_group = res->findByNameRec("IPv6_STR_PATTERN_OR_GROUP");

    if (!res_or_group)
    {
        return nullptr;
    }

    if (res_or_group->error)
    {
        return res_or_group->error;
    }

    if (!res_or_group->matched)
    {
        return nullptr;
    }

    auto ms = res_or_group->getMatchedString();

    std::deque<UString> ms_spl;
    ms.split(ms_spl, ":");

    ipv6_v4_comb = false;

    auto res_comb = res->findByNameRec("IPv4_STR_PATTERN");
    if (res_comb)
    {
        if (res_comb->error)
        {
            return res_comb->error;
        }

        if (!res_comb->matched)
        {
            return nullptr;
        }

        ipv6_v4_comb = true;
    }

    if (ipv6_v4_comb)
    {
        union
        {
            std::array<std::uint16_t, 2> ipv6_ints;
            std::array<std::uint8_t, 4>  ipv4_bytes;
        };

        bool ipv4_bytes_matched;

        err = getValuesFrom_IPv4_STR_PATTERN_Result(
            res_comb,
            ipv4_bytes_matched,
            ipv4_bytes
        );
        if (err)
        {
            return err;
        }

        if (!ipv4_bytes_matched)
        {
            return wayround_i2p::ccutils::errors::New(
                "ipv6_ipv4_comb_dismatch",
                __FILE__,
                __LINE__
            );
        }

        ipv6[0] = ipv6_ints[0];
        ipv6[1] = ipv6_ints[1];

        for (unsigned char i = 0; i < 4; i++)
        {
            ipv4[i] = ipv4_bytes[i];
        }
    }

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

    if (ipv6_short)
    {
        while (ms_spl.size() != (ipv6_v4_comb ? 7 : 8))
        {
            short_index_itr = ms_spl.insert(short_index_itr, "");
        }
    }

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

    if (ret_bigendian && std::endian::native == std::endian::little)
    {
        for (unsigned char i = (ipv6_v4_comb ? 2 : 0); i < 8; i++)
        {
            ipv6[i] = std::byteswap(ipv6[i]);
        }
    }

    matched = true;

    return nullptr;
}

error_ptr getAllPossibleValuesFromResult(
    const regexp::Result_shared   res,
    IPver                        &ipver,
    bool                         &ipv6_matched,
    std::array<std::uint16_t, 8> &ipv6,
    bool                         &ipv4_matched,
    std::array<std::uint8_t, 4>  &ipv4,
    bool                         &ipv6_short,
    bool                         &ipv6_v4_comb,
    bool                         &port_matched,
    std::uint16_t                &port,
    bool                         &cidr_matched,
    std::uint16_t                &cidr,
    bool                          ret_bigendian
)
{
    error_ptr err;

    ipv6_matched = false;
    ipv4_matched = false;
    port_matched = false;
    cidr_matched = false;

    ipver = IPver::invalid;

    err = getValuesFrom_IPv6_STR_PATTERN_Result(
        res,
        ipv6_matched,
        ipv6,
        ipv4,
        ipv6_short,
        ipv6_v4_comb,
        ret_bigendian
    );
    if (err)
    {
        return err;
    }

    if (ipv6_matched)
    {
        ipver = IPver::v6;
        goto after_ip_test;
    }

    err = getValuesFrom_IPv4_STR_PATTERN_Result(
        res,
        ipv4_matched,
        ipv4
    );
    if (err)
    {
        return err;
    }

    if (ipv4_matched)
    {
        ipver = IPver::v4;
        goto after_ip_test;
    }

after_ip_test:
    err = nullptr;

    err = getValueFrom_PORT_STR_PATTERN_Result(
        res,
        port_matched,
        port
    );

    err = getValueFrom_CIDR_STR_PATTERN_Result(
        res,
        cidr_matched,
        cidr
    );

    return nullptr;
}

IP::IP()
{
    clear();
}

IP::~IP()
{
}

IP_shared IP::create()
{
    auto ret     = IP_shared(new IP());
    ret->own_ptr = ret;
    return ret;
}

void IP::clear()
{
    ipver = IPver::invalid;

    has_ip   = false;
    has_port = false;
    has_cidr = false;

    ipv6_v4_comb = false;

    resetIPfield();
    port = 0;
    cidr = 0;
}

void IP::resetIPfield()
{
    buff.ipv6.b32 = {0, 0, 0, 0};
}

error_ptr IP::setAllFromString(const UString &text)
{
    auto pat = OPT_IP_AND_OPT_PORT_OR_CIDR_PATTERN();

    auto res = pat->match(std::shared_ptr<UString>(new UString(text)));

    bool ipv6_matched;
    bool ipv4_matched;
    bool ipv6_short;

    auto err = getAllPossibleValuesFromResult(
        res,
        ipver,
        ipv6_matched,
        buff.ipv6.b16,
        ipv4_matched,
        buff.ipv4,
        ipv6_short,
        ipv6_v4_comb,
        has_port,
        port,
        has_cidr,
        cidr,
        true
    );

    if (err)
    {
        return err;
    }

    has_ip = ipv6_matched || ipv4_matched;

    if (ipv6_matched)
    {
        ipver = IPver::v6;
    }

    if (ipv4_matched)
    {
        ipver = IPver::v4;
    }

    return nullptr;
}

void IP::setAllFromIP(const IP_shared obj)
{
    setIPFromIP(obj);
    setPortFromIP(obj);
    setCIDRFromIP(obj);
}

void IP::setIPFromIP(const IP_shared obj)
{
    delIP();

    has_ip = obj->has_ip;

    if (has_ip)
    {
        ipver        = obj->ipver;
        ipv6_v4_comb = obj->ipv6_v4_comb;
        switch (ipver)
        {
            default:
                throw "programming error";
            case IPver::v4:
                buff.ipv4 = obj->buff.ipv4;
                break;
            case IPver::v6:
                buff.ipv6 = obj->buff.ipv6;
                break;
        }
    }
}

void IP::setPortFromIP(const IP_shared obj)
{
    has_port = obj->has_port;
    port     = obj->port;
}

void IP::setCIDRFromIP(const IP_shared obj)
{
    has_cidr = obj->has_cidr;
    cidr     = obj->cidr;
}

void IP::setIPFromArray(const std::array<std::uint8_t, 4> &arr)
{
    has_ip = true;
    ipver  = IPver::v4;

    resetIPfield();

    buff.ipv4 = arr;
}

void IP::setIPFromArray(
    const std::array<std::uint8_t, 16> &arr,
    bool                                detect_v4_magic
)
{
    has_ip = true;
    ipver  = IPver::v6;

    buff.ipv6.b8 = arr;

    if (detect_v4_magic)
    {
        reset_ipv6_v4_comb_basing_on_magic();
    }
}

void IP::setIPFromArray(
    const std::array<std::uint16_t, 8> &arr,
    bool                                big,
    bool                                detect_v4_magic
)
{
    has_ip = true;
    ipver  = IPver::v6;

    if (!big && std::endian::native == std::endian::little)
    {
        for (unsigned char i = 0; i < 8; i++)
        {
            buff.ipv6.b16[i] = std::byteswap(arr[i]);
        }
    }
    else
    {
        buff.ipv6.b16 = arr;
    }

    if (detect_v4_magic)
    {
        reset_ipv6_v4_comb_basing_on_magic();
    }
}

void IP::setIPFromArray(
    const std::array<std::uint32_t, 4> &arr,
    bool                                big,
    bool                                detect_v4_magic
)
{
    has_ip = true;
    ipver  = IPver::v6;

    if (!big && std::endian::native == std::endian::little)
    {
        for (unsigned char i = 0; i < 4; i++)
        {
            buff.ipv6.b32[i] = std::byteswap(arr[i]);
        }
    }
    else
    {
        buff.ipv6.b32 = arr;
    }

    if (detect_v4_magic)
    {
        reset_ipv6_v4_comb_basing_on_magic();
    }
}

error_ptr IP::setIPFromVector(const std::vector<std::uint8_t> &vec)
{
    union
    {
        std::array<std::uint8_t, 4>  arr4;
        std::array<std::uint8_t, 16> arr6;
    };

    auto vec_size = vec.size();
    switch (vec_size)
    {
        case 4:
        {
            for (unsigned char i = 0; i < 4; i++)
            {
                arr4[i] = vec[i];
            }

            setIPFromArray(arr4);
            return nullptr;
        }

        case 16:
        {
            for (unsigned char i = 0; i < 16; i++)
            {
                arr6[i] = vec[i];
            }

            setIPFromArray(arr6);
            return nullptr;
        }

        default:
        {
            return wayround_i2p::ccutils::errors::New(
                "setIPFromVector: invalid uint8_t vector size",
                __FILE__,
                __LINE__
            );
        }
    }
}

error_ptr IP::setIPFromVector(const std::vector<std::uint16_t> &vec, bool big)
{
    if (vec.size() != 8)
    {
        return wayround_i2p::ccutils::errors::New(
            "setIPFromVector: invalid uint16_t vector size",
            __FILE__,
            __LINE__
        );
    }

    std::array<std::uint16_t, 8> arr;

    for (unsigned char i = 0; i < 8; i++)
    {
        arr[i] = vec[i];
    }

    setIPFromArray(arr, big);

    return nullptr;
}

error_ptr IP::setIPFromVector(const std::vector<std::uint32_t> &vec, bool big)
{
    if (vec.size() != 4)
    {
        return wayround_i2p::ccutils::errors::New(
            "setIPFromVector: invalid uint32_t vector size",
            __FILE__,
            __LINE__
        );
    }

    std::array<std::uint32_t, 4> arr;

    for (unsigned char i = 0; i < 4; i++)
    {
        arr[i] = vec[i];
    }

    setIPFromArray(arr, big);

    return nullptr;
}

error_ptr IP::setIPFromString(const UString &text)
{
    resetIPfield();

    auto pat = OPT_IP_AND_OPT_PORT_OR_CIDR_PATTERN();

    auto res = pat->match(std::shared_ptr<UString>(new UString(text)));

    IPver                        ipver;
    bool                         ipv6_matched;
    std::array<std::uint16_t, 8> ipv6;
    bool                         ipv4_matched;
    std::array<std::uint8_t, 4>  ipv4;
    bool                         ipv6_short;
    bool                         ipv6_v4_comb;
    bool                         port_matched;
    std::uint16_t                port;
    bool                         cidr_matched;
    std::uint16_t                cidr;

    auto err = getAllPossibleValuesFromResult(
        res,
        ipver,
        ipv6_matched,
        ipv6,
        ipv4_matched,
        ipv4,
        ipv6_short,
        ipv6_v4_comb,
        has_port,
        port,
        has_cidr,
        cidr,
        false
    );
    if (err)
    {
        return err;
    }

    if (has_port || has_cidr)
    {
        return wayround_i2p::ccutils::errors::New(
            "setIPFromString: IP only expected in string",
            __FILE__,
            __LINE__
        );
    }

    if (ipv6_matched)
    {
        setIPFromArray(ipv6);
        return nullptr;
    }

    if (ipv4_matched)
    {
        setIPFromArray(ipv4);
        return nullptr;
    }

    return wayround_i2p::ccutils::errors::New(
        "setIPFromString: IP not recognized",
        __FILE__,
        __LINE__
    );
}

void IP::setPort(std::uint16_t val)
{
    delCIDR();
    has_port = true;
    port     = val;
}

void IP::setCIDR(std::uint16_t val)
{
    delPort();
    has_cidr = true;
    cidr     = val;
}

IPver IP::getIPver() const
{
    return ipver;
}

bool IP::hasIP() const
{
    return has_ip;
}

bool IP::hasIPv4() const
{
    return has_ip && ipver == IPver::v4;
}

bool IP::hasIPv6() const
{
    return has_ip && ipver == IPver::v6;
}

bool IP::hasPort() const
{
    return has_port;
}

bool IP::hasCIDR() const
{
    return has_cidr;
}

void IP::delIP()
{
    has_ip = false;
    resetIPfield();
    ipver = IPver::invalid;
}

void IP::delPort()
{
    has_port = false;
    port     = 0;
}

void IP::delCIDR()
{
    has_cidr = false;
    cidr     = 0;
}

bool IP::isIPv6IPv4combine(bool true_if_magic, bool true_if_flag) const
{
    if (!hasIPv6())
    {
        return false;
    }

    if (true_if_magic && hasIPv6IPv4CombineMagic())
    {
        return true;
    }

    if (true_if_flag && ipv6_v4_comb)
    {
        return true;
    }

    return false;
}

bool IP::hasIPv6IPv4CombineMagic(bool ensure_has_ip_and_ip_is_ipv6) const
{
    if (ensure_has_ip_and_ip_is_ipv6 && !hasIPv6())
    {
        return false;
    }

    return buff.ipv6.b16[2] == 0xffff;
}

void IP::reset_ipv6_v4_comb_basing_on_magic(bool ensure_has_ip_and_ip_is_ipv6)
{
    setIPv6IPv4Combine(hasIPv6IPv4CombineMagic(ensure_has_ip_and_ip_is_ipv6));
}

void IP::setIPv6IPv4Combine(bool val, bool set_ipv4_combine_magic)
{
    ipv6_v4_comb = val;

    if (set_ipv4_combine_magic && val)
    {
        applyIPv6IPv4CombineMagic(false);
    }
}

void IP::applyIPv6IPv4CombineMagic(bool also_set_ipv6_v4_comb_true)
{
    buff.ipv6.b16[2] = 0xffff;
    if (also_set_ipv6_v4_comb_true)
    {
        ipv6_v4_comb = true;
    }
}

void IP::setIPv6IPv4Part(const std::array<std::uint8_t, 4> &arr)
{
    for (unsigned char i = 0; i < 4; i++)
    {
        buff.ipv4[i] = arr[i];
    }
}

void IP::setIPv6IPv4Part(IP_shared obj)
{
    buff.ipv4 = obj->buff.ipv4;
}

IP_shared IP::getIPv6IPv4Part() const
{
    auto ret = IP::create();
    ret->setIPFromArray(buff.ipv4);
    return ret;
}

std::array<std::uint8_t, 4> &IP::getIPAsArray8(std::array<std::uint8_t, 4> &arr) const
{
    for (unsigned char i = 0; i < 4; i++)
    {
        arr[i] = buff.ipv4[i];
    }
    return arr;
}

std::array<std::uint8_t, 16> &IP::getIPAsArray8(std::array<std::uint8_t, 16> &arr) const
{
    for (unsigned char i = 0; i < 16; i++)
    {
        arr[i] = buff.ipv6.b8[i];
    }
    return arr;
}

std::array<std::uint16_t, 8> &IP::getIPAsArray16(
    std::array<std::uint16_t, 8> &arr,
    bool                          big
) const
{
    for (unsigned char i = 0; i < 8; i++)
    {
        auto x = buff.ipv6.b16[i];
        if (!big && std::endian::native == std::endian::little)
        {
            x = std::byteswap(arr[i]);
        }
        arr[i] = x;
    }
    return arr;
}

std::array<std::uint32_t, 4> &IP::getIPAsArray32(
    std::array<std::uint32_t, 4> &arr,
    bool                          big
) const
{
    for (unsigned char i = 0; i < 4; i++)
    {
        auto x = buff.ipv6.b32[i];
        if (!big && std::endian::native == std::endian::little)
        {
            x = std::byteswap(x);
        }
        arr[i] = x;
    }
    return arr;
}

std::vector<std::uint8_t> &IP::getIPAsVector8(std::vector<std::uint8_t> &vec) const
{
    if (!has_ip)
    {
        vec.resize(0);
        return vec;
    }

    switch (ipver)
    {
        case IPver::v4:
        {
            vec.resize(4);
            for (unsigned char i = 0; i < 4; i++)
            {
                vec[i] = buff.ipv4[i];
            }
            return vec;
        }

        case IPver::v6:
        {
            vec.resize(16);
            for (unsigned char i = 0; i < 16; i++)
            {
                vec[i] = buff.ipv6.b8[i];
            }
            return vec;
        }

        default:
        {
            vec.resize(0);
            return vec;
        }
    }
}

std::vector<std::uint16_t> &IP::getIPAsVector16(
    std::vector<std::uint16_t> &vec,
    bool                        big
) const
{
    if (!has_ip)
    {
        vec.resize(0);
        return vec;
    }

    switch (ipver)
    {
        case IPver::v6:
        {
            vec.resize(8);
            for (unsigned char i = 0; i < 8; i++)
            {
                vec[i] = buff.ipv6.b16[i];
            }
            return vec;
        }

        default:
        {
            vec.resize(0);
            return vec;
        }
    }
}

std::vector<std::uint32_t> &IP::getIPAsVector32(
    std::vector<std::uint32_t> &vec,
    bool                        big
) const
{
    if (!has_ip)
    {
        vec.resize(0);
        return vec;
    }

    switch (ipver)
    {
        case IPver::v6:
        {
            vec.resize(4);
            for (unsigned char i = 0; i < 4; i++)
            {
                vec[i] = buff.ipv6.b32[i];
            }
            return vec;
        }

        default:
        {
            vec.resize(0);
            return vec;
        }
    }
}

std::uint16_t IP::getPort() const
{
    if (!has_port)
    {
        return 0;
    }
    return port;
}

std::uint16_t IP::getCIDR() const
{
    if (!has_cidr)
    {
        return 0;
    }
    return cidr;
}

UString IP::getAllAsString() const
{
    // UString ip;
    UString port_or_cidr;

    if (has_port)
    {
        port_or_cidr = getPortString();
    }
    else if (has_cidr)
    {
        port_or_cidr = getCIDRString();
    }
    else
    {
        port_or_cidr = "";
    }

    UString ret;

    if (has_ip)
    {
        ret = getIPAsString();
    }

    if ((ret != "" && ipver == IPver::v6)
        && has_port
        && (port_or_cidr != ""))
    {
        ret = UString("[") + ret + "]";
    }

    ret += port_or_cidr;

    return ret;
}

UString IP::getIPAsString() const
{
    if (!has_ip)
    {
        return "";
    }

    switch (ipver)
    {
        case IPver::v4:
            return getIPv4AsString();
        case IPver::v6:
            return getIPv6AsString();
        default:
            throw wayround_i2p::ccutils::errors::New(
                "getIPAsString: invalid ipver",
                __FILE__,
                __LINE__
            );
    }
}

UString IP::getIPv4AsString() const
{
    return std::format(
        "{}.{}.{}.{}",
        buff.ipv4[3],
        buff.ipv4[2],
        buff.ipv4[1],
        buff.ipv4[0]
    );
}

UString IP::getIPv6AsString() const
{
    return getIPv6AsStringShort();
}

UString IP::getIPv6AsStringLong(bool leading_zeroes) const
{
    UString ret;

    const std::string fmt = (leading_zeroes ? "{:04x}" : "{:01x}");

    unsigned char i = 7;

    while (true)
    {
        auto z = buff.ipv6.b16[i];
        if constexpr (std::endian::native == std::endian::little)
        {
            z = std::byteswap(z);
        }
        ret += std::vformat(fmt, std::make_format_args(z));
        if (i >= ((ipv6_v4_comb ? 2 : 0) + 1))
        {
            ret += ":";
        }

        if (i <= (ipv6_v4_comb ? 2 : 0))
        {
            break;
        }
        i--;
    }

    if (ipv6_v4_comb)
    {
        ret += std::format(":{}", getIPv6IPv4Part()->getAllAsString());
    }

    return ret;
}

UString IP::getIPv6AsStringShort(bool leading_zeroes) const
{
    const std::string fmt = (leading_zeroes ? "{:04x}" : "{:01x}");

    struct zeroes_slice
    {
        std::size_t start;
        std::size_t length;
    };

    std::deque<zeroes_slice> slices;

    {
        bool        zeroes_slice_started = false;
        std::size_t zeroes_slice_i       = -1;

        for (unsigned char i = (ipv6_v4_comb ? 2 : 0); i < 8; i++)
        {
            if (buff.ipv6.b16[i] == 0)
            {
                if (zeroes_slice_started)
                {
                    auto &x = slices[zeroes_slice_i];
                    x.length++;
                }
                else
                {
                    zeroes_slice_i++;
                    slices.push_back(zeroes_slice{i, 1});
                    zeroes_slice_started = true;
                }
            }
            else
            {
                if (zeroes_slice_started)
                {
                    zeroes_slice_started = false;
                }
            }
        }
    }

    if (slices.size() == 0)
    {
        return getIPv6AsStringLong(false);
    }

    zeroes_slice *longest = &(slices[0]);

    for (std::size_t i = 1; i < slices.size(); i++)
    {
        if (slices[i].length > (*longest).length)
        {
            longest = &(slices[i]);
        }
    }

    zeroes_slice &longest_np = *longest;

    UString ret;

    if (ipv6_v4_comb)
    {
        ret = std::format(":{}", getIPv6IPv4Part()->getAllAsString());
    }

    {
        if (longest_np.start == 0)
        {
            ret = UString(":") + ret;
        }
        else
        {
            for (
                std::size_t i = (ipv6_v4_comb ? 2 : 0);
                i < longest_np.start;
                i++
            )
            {
                auto z = buff.ipv6.b16[i];

                if constexpr (std::endian::native == std::endian::little)
                {
                    z = std::byteswap(z);
                }

                if (i == (ipv6_v4_comb ? 2 : 0) && ret != "")
                {
                    ret = UString(":") + ret;
                }

                ret = UString(std::vformat(fmt, std::make_format_args(z))) + ret;
                ret = UString(":") + ret;
            }
        }

        if ((longest_np.start + longest_np.length) == 8)
        {
            ret = UString(":") + ret;
        }
        else
        {
            for (
                std::size_t i = longest_np.start + longest_np.length;
                i < 8;
                i++
            )
            {
                auto z = buff.ipv6.b16[i];

                if constexpr (std::endian::native == std::endian::little)
                {
                    z = std::byteswap(z);
                }

                ret = UString(":") + ret;
                ret = UString(std::vformat(fmt, std::make_format_args(z))) + ret;
            }
        }
    }
    return ret;
}

UString IP::getPortString() const
{
    return std::format(":{}", getPort());
}

UString IP::getCIDRString() const
{
    return std::format("/{}", getCIDR());
}

UString IP::debugRepr() const
{
    UString ret = std::format(
        "text: \"{}\"\n"
        "  ipv6_matched : {}\n"
        "  ipv6         : {}\n"
        "  ipv4_matched : {}\n"
        "  ipv4         : {}\n"
        "  ipv6_v4_comb : {}\n"
        "  has_port     : {}\n"
        "  port         : {}\n"
        "  has_cidr     : {}\n"
        "  cidr         : {}\n",
        getAllAsString().to_string(),
        ipver == IPver::v6,
        (ipver == IPver::v6 ? getIPv6AsString().to_string() : "-"),
        ipver == IPver::v4,
        (ipver == IPver::v4 ? getIPv4AsString().to_string() : "-"),
        ipv6_v4_comb,
        has_port,
        port,
        has_cidr,
        cidr
    );

    return ret;
}

} // namespace wayround_i2p::ccutils::ip
