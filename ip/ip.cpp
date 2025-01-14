
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

    err = regexp::ResultRoutineCheck(
        res_VALUE_STR_PATTERN,
        true,
        false,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    if (!res_VALUE_STR_PATTERN->matched)
    {
        return nullptr;
    }

    auto res_number = res_VALUE_STR_PATTERN->findByNameRec("number");

    err = regexp::ResultRoutineCheck(
        res_number,
        true,
        true,
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

    auto res3 = res->findByName("IPv4_STR_PATTERN");

    err = regexp::ResultRoutineCheck(
        res3,
        true,
        false,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    if (!res3->matched)
    {
        return nullptr;
    }

    for (unsigned char i = 0; i < 4; i++)
    {
        auto res2 = res3->findByNameRec(std::to_string(i + 1));

        err = regexp::ResultRoutineCheck(
            res2,
            true,
            true,
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
    bool                          ret_localendian
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

    err = regexp::ResultRoutineCheck(
        res_or_group,
        true,
        false,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    if (!res_or_group->matched)
    {
        return nullptr;
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
    bool                          ret_localendian
)
{
    error_ptr err;

    ipv6_matched = false;
    ipv4_matched = false;
    port_matched = false;
    cidr_matched = false;

    err = getValuesFrom_IPv6_STR_PATTERN_Result(
        res,
        ipv6_matched,
        ipv6,
        ipv4,
        ipv6_short,
        ipv6_v4_comb,
        ret_localendian
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

error_ptr IP::setAllFromString(const UString &text)
{
    auto pat = OPT_IP_AND_MUST_PORT_OR_CIDR_PATTERN();

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
        false
    );

    if (err)
    {
        return err;
    }

    has_ip = ipv6_matched || ipv4_matched;

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
    this->buff = obj->buff;
}

void IP::setPortFromIP(const IP_shared obj);

void IP::setCIDRFromIP(const IP_shared obj);

void IP::setIPFromArray(const std::array<std::uint8_t, 4> &arr);
void IP::setIPFromArray(const std::array<std::uint8_t, 16> &arr);
void IP::setIPFromArray(const std::array<std::uint16_t, 8> &arr, bool big = false);
void IP::setIPFromArray(const std::array<std::uint32_t, 4> &arr, bool big = false);

error_ptr IP::setIPFromVector(const std::vector<std::uint8_t> &vec);
error_ptr IP::setIPFromVector(const std::vector<std::uint16_t> &vec, bool big = false);
error_ptr IP::setIPFromVector(const std::vector<std::uint32_t> &vec, bool big = false);

error_ptr IP::setIPFromString(const UString &text);

// automativaly deletes CIDR
void IP::setPort(std::uint16_t val);

// automativaly deletes Port
void IP::setCIDR(std::uint16_t val);

IPver IP::getIPver() const;

bool IP::hasIP() const;
bool IP::hasIPv4() const;
bool IP::hasIPv6() const;
bool IP::hasPort() const;
bool IP::hasCIDR() const;

bool IP::isIPv6IPv4combine() const;

// returns true if ipv6 is stored insude and 3rd uint16 equals ffff.
bool IP::hasIPv6IPv4CombineMagic() const;

void IP::setIPv6IPv4Combine(bool val, bool set_ipv4_combine_magic = true);

// note: this doesn't automatically sets IPv6IPv4combine to true.
//       use setIPv6IPv4combine() function
void IP::setIPv6IPv4Part(const std::array<std::uint8_t, 4> &arr);
void IP::setIPv6IPv4Part(const IP &obj);

IP_shared IP::getIPv6IPv4Part() const;

std::array<std::uint8_t, 4>  &IP::getIPAsArray8(std::array<std::uint8_t, 4> &arr) const;
std::array<std::uint8_t, 16> &IP::getIPAsArray8(std::array<std::uint8_t, 16> &arr) const;
std::array<std::uint16_t, 8> &IP::getIPAsArray16(std::array<std::uint16_t, 8> &arr, bool big = false) const;
std::array<std::uint32_t, 4> &IP::getIPAsArray32(std::array<std::uint32_t, 4> &arr, bool big = false) const;

std::vector<std::uint8_t>  &IP::getIPAsVector8(std::vector<std::uint8_t> &vec) const;
std::vector<std::uint16_t> &IP::getIPAsVector16(std::vector<std::uint16_t> &vec, bool big = false) const;
std::vector<std::uint32_t> &IP::getIPAsVector32(std::vector<std::uint32_t> &vec, bool big = false) const;

std::tuple<std::uint16_t, error_ptr> IP::getPort() const;
std::tuple<std::uint16_t, error_ptr> IP::getCIDR() const;

UString IP::getAllAsString() const;
UString IP::getAllAsLongString() const;
UString IP::getAllAsShortString() const;

UString IP::getIPAsString() const;
UString IP::getIPAsStringLong() const;
UString IP::getIPAsStringShort() const;

UString IP::getPortString() const;
UString IP::getCIDRString() const;

} // namespace wayround_i2p::ccutils::ip
