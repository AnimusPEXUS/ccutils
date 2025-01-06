
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

    std::cout << res->repr_as_text(true) << std::endl;

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

IPv4_shared IPv4::create()
{
    auto ret     = IPv4_shared(new IPv4());
    ret->own_ptr = ret;
    return ret;
}

// see setFromArray() for format explanation
IPv4_shared IPv4::createFromArray(const std::array<std::uint8_t, 4> &arr)
{
    auto ret = IPv4::create();
    ret->setFromArray(arr);
    return ret;
}

// see setFromArray() for format explanation
std::tuple<IPv4_shared, error_ptr> IPv4::createFromVector(const std::vector<std::uint8_t> &vec)
{
    auto ret = IPv4::create();
    auto err = ret->setFromVector(vec);
    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

// see setFromArray() for format explanation
std::tuple<IPv4_shared, error_ptr> IPv4::createFromString(const UString &val)
{
    auto ret = IPv4::create();
    auto err = ret->setFromString(val);
    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

std::tuple<IPv4_shared, error_ptr> IPv4::createFromParsedString(const regexp::Result_shared &res)
{
    auto ret = IPv4::create();
    auto err = ret->setFromParsedString(res);
    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

///
// (in text form, smaller value is on the right)
// 192.168.0.1
//   3   2 1 0
//      |
//      v
// arr[3] arr[2] arr[1] arr[0]
void IPv4::setFromArray(const std::array<std::uint8_t, 4> &arr)
{
    buff = arr;
}

// see setFromArray() for format explanation
error_ptr IPv4::setFromVector(const std::vector<std::uint8_t> &vec)
{
    if (vec.size() != 4)
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid vector size",
            __FILE__,
            __LINE__
        );
    }

    for (unsigned char i = 0; i < 4; i++)
    {
        buff[i] = vec[i];
    }

    return nullptr;
}

// see setFromArray() for format explanation
error_ptr IPv4::setFromString(const UString &val)
{

    auto pat = IPv4_STR_PATTERN();
    auto res = pat->match(std::shared_ptr<UString>(new UString(val)));

    auto err = setFromParsedString(res);
    if (err)
    {
        return err;
    }

    return nullptr;
}

error_ptr IPv4::setFromParsedString(const regexp::Result_shared &res)
{
    std::array<std::uint8_t, 4> tmp;

    auto err = getValuesFrom_IPv4_STR_PATTERN_Result(res, tmp);
    if (err)
    {
        return err;
    }

    setFromArray(tmp);
    return nullptr;
}

UString IPv4::toString() const
{
    return std::format("{}.{}.{}.{}", buff[3], buff[2], buff[1], buff[0]);
}

std::array<std::uint8_t, 4> IPv4::toArray() const
{
    return buff;
}

std::vector<std::uint8_t> IPv4::toVector() const
{
    std::vector<std::uint8_t> ret(4);
    for (unsigned char i = 0; i < 4; i++)
    {
        ret[i] = buff[i];
    }
    return ret;
}

IPv4::IPv4()
{
}

IPv4::~IPv4()
{
}

IPv6_shared IPv6::create()
{
    auto ret     = IPv6_shared(new IPv6());
    ret->own_ptr = ret;
    return ret;
}

IPv6_shared IPv6::createFromArray(const std::array<std::uint8_t, 16> &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}

IPv6_shared IPv6::createFromArray(const std::array<std::uint16_t, 8> &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}

/*
IPv6_shared IPv6::createFromArray(const IPv6_array &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}
*/

IPv6_shared IPv6::createFromArray(const std::array<std::uint32_t, 4> &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}

std::tuple<IPv6_shared, error_ptr> IPv6::createFromVector(const std::vector<std::uint8_t> &vec)
{
    auto ret = IPv6::create();
    auto err = ret->setFromVector(vec);
    if (err)
    {
        return {nullptr, err};
    }
    return {ret, nullptr};
}

std::tuple<IPv6_shared, error_ptr> IPv6::createFromVector(const std::vector<std::uint16_t> &vec)
{
    auto ret = IPv6::create();
    auto err = ret->setFromVector(vec);
    if (err)
    {
        return {nullptr, err};
    }
    return {ret, nullptr};
}

std::tuple<IPv6_shared, error_ptr> IPv6::createFromVector(const std::vector<std::uint32_t> &vec)
{
    auto ret = IPv6::create();
    auto err = ret->setFromVector(vec);
    if (err)
    {
        return {nullptr, err};
    }
    return {ret, nullptr};
}

std::tuple<IPv6_shared, error_ptr> IPv6::createFromString(const UString &text)
{
    auto ret = IPv6::create();
    auto err = ret->setFromString(text);
    if (err)
    {
        return {nullptr, err};
    }
    return {ret, nullptr};
}

void IPv6::setFromArray(const std::array<std::uint8_t, 16> &arr)
{
    buff.b8 = arr;
}

// note: endianness of ints must be local. function converts endiannes manually if needed
void IPv6::setFromArray(const std::array<std::uint16_t, 8> &arr)
{
    if constexpr (std::endian::native == std::endian::little)
    {
        std::array<std::uint16_t, 8> new_arr;
        for (unsigned char i = 0; i < 8; i++)
        {
            buff.b16[i] = std::byteswap(arr[i]);
        }
    }
    else
    {
        buff.b16 = arr;
    }
}

// note: endianness of ints must be local. function converts endiannes manually if needed
void IPv6::setFromArray(const std::array<std::uint32_t, 4> &arr)
{
    if constexpr (std::endian::native == std::endian::little)
    {
        std::array<std::uint32_t, 4> new_arr;
        for (unsigned char i = 0; i < 4; i++)
        {
            buff.b32[i] = std::byteswap(arr[i]);
        }
    }
    else
    {
        buff.b32 = arr;
    }
}

/*
void IPv6::setFromArray(const IPv6_array &arr)
{
    buff = arr;
}
*/

error_ptr IPv6::setFromVector(const std::vector<std::uint8_t> &vec)
{
    if (vec.size() != 16)
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid vector size",
            __FILE__,
            __LINE__
        );
    }

    for (unsigned char i = 0; i != 16; i++)
    {
        buff.b8[i] = vec[i];
    }

    return nullptr;
}

error_ptr IPv6::setFromVector(const std::vector<std::uint16_t> &vec)
{
    if (vec.size() != 8)
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid vector size",
            __FILE__,
            __LINE__
        );
    }

    for (unsigned char i = 0; i != 8; i++)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            buff.b16[i] = std::byteswap(vec[i]);
        }
        else
        {
            buff.b16[i] = vec[i];
        }
    }

    return nullptr;
}

error_ptr IPv6::setFromVector(const std::vector<std::uint32_t> &vec)
{
    if (vec.size() != 4)
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid vector size",
            __FILE__,
            __LINE__
        );
    }

    for (unsigned char i = 0; i < 4; i++)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            buff.b32[i] = std::byteswap(vec[i]);
        }
        else
        {
            buff.b32[i] = vec[i];
        }
    }

    return nullptr;
}

error_ptr IPv6::setFromString(const UString &text)
{
    std::array<std::uint16_t, 8> tmp;

    auto pat = IPv6_STR_PATTERN();
    auto res = pat->match(
        std::shared_ptr<UString>(
            new UString(text)
        )
    );

    bool ipv6_short = false;

    auto err = getValuesFrom_IPv6_STR_PATTERN_Result(
        res,
        tmp,
        ipv6_short,
        ipv4_comb
    );
    if (err)
    {
        return err;
    }

    setFromArray(tmp);
    return nullptr;
}

/*
IPv6_array IPv6::toArray() const
{
    return buff;
}
*/

std::array<std::uint8_t, 16> &IPv6::toArray8(std::array<std::uint8_t, 16> &arr) const
{
    for (unsigned char i = 0; i < 16; i++)
    {
        arr[i] = buff.b8[i];
    }
    return arr;
}

std::array<std::uint16_t, 8> &IPv6::toArray16(std::array<std::uint16_t, 8> &arr) const
{
    for (unsigned char i = 0; i < 8; i++)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            arr[i] = std::byteswap(buff.b16[i]);
        }
        else
        {
            arr[i] = buff.b16[i];
        }
    }
    return arr;
}

std::array<std::uint32_t, 4> &IPv6::toArray32(std::array<std::uint32_t, 4> &arr) const
{
    for (unsigned char i = 0; i < 4; i++)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            arr[i] = std::byteswap(buff.b32[i]);
        }
        else
        {
            arr[i] = buff.b32[i];
        }
    }
    return arr;
}

std::vector<std::uint8_t> &IPv6::toVector8(std::vector<std::uint8_t> &vec) const
{
    for (unsigned char i = 0; i < 16; i++)
    {
        vec[i] = buff.b8[i];
    }
    return vec;
}

std::vector<std::uint16_t> &IPv6::toVector16(std::vector<std::uint16_t> &vec) const
{
    for (unsigned char i = 0; i < 8; i++)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            vec[i] = std::byteswap(buff.b16[i]);
        }
        else
        {
            vec[i] = buff.b16[i];
        }
    }
    return vec;
}

std::vector<std::uint32_t> &IPv6::toVector32(std::vector<std::uint32_t> &vec) const
{
    for (unsigned char i = 0; i < 4; i++)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            vec[i] = std::byteswap(buff.b32[i]);
        }
        else
        {
            vec[i] = buff.b32[i];
        }
    }
    return vec;
}

UString IPv6::toString() const
{
    return toStringShort();
}

UString IPv6::toStringLong() const
{
    UString ret;

    for (unsigned char i = 0; i < 8; i++)
    {
        auto z = buff.b16[7 - i];
        if constexpr (std::endian::native == std::endian::little)
        {
            z = std::byteswap(z);
        }
        ret += std::format("{:04x}", z);
        if (i < 7)
        {
            ret += ":";
        }
    }

    return ret;
}

UString IPv6::toStringShort() const
{
    struct zeroes_slice
    {
        std::size_t start;
        std::size_t length;
    };

    std::deque<zeroes_slice> slices;

    {
        bool        zeroes_slice_started = false;
        std::size_t zeroes_slice_i       = -1;

        for (unsigned char i = 0; i < 8; i++)
        {
            if (buff.b16[i] == 0)
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

    /*
    std::cout << "std::deque<zeroes_slice> slices;" << std::endl;

    for (std::size_t i = 0; i < slices.size(); i++)
    {
        const auto &x = *(slices.begin() + i);
        std::cout << std::format("  slice(start={}, length={})", x.start, x.length) << std::endl;
    }
    */

    if (slices.size() == 0)
    {
        return toStringLong();
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

    {
        if (longest_np.start == 0)
        {
            ret = UString(":") + ret;
        }
        else
        {
            for (
                std::size_t i = 0;
                i < longest_np.start;
                i++
            )
            {
                auto z = buff.b16[i];

                if constexpr (std::endian::native == std::endian::little)
                {
                    z = std::byteswap(z);
                }

                ret = UString(std::vformat("{:04x}", std::make_format_args(z))) + ret;
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
                auto z = buff.b16[i];

                if constexpr (std::endian::native == std::endian::little)
                {
                    z = std::byteswap(z);
                }

                ret = UString(":") + ret;
                ret = UString(std::vformat("{:04x}", std::make_format_args(z))) + ret;
            }
        }
    }
    return ret;
}

void IPv6::setIPv4Comb(bool val)
{
    ipv4_comb = val;
}

void IPv6::setIPv4Comb(const IPv4_shared &comb_part)
{
    auto arr = comb_part->toArray();
    for (unsigned char i = 0; i < 4; i++)
    {
        buff.b8[i + 12] = arr[i];
    }
}

bool IPv6::isIPv4Comb() const
{
    return ipv4_comb;
}

IPv4_shared IPv6::getIPv4Comb() const
{
    std::array<std::uint8_t, 4> arr;
    for (unsigned char i = 0; i < 4; i++)
    {
        arr[i] = buff.b8[i + 12];
    }
    return IPv4::createFromArray(arr);
}

IPv6::IPv6()
{
    for (unsigned char i = 0; i < 16; i++)
    {
        buff.b8[i] = 0;
    }
}

IPv6::~IPv6()
{
}

} // namespace wayround_i2p::ccutils::ip
