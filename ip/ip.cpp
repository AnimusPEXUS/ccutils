
#include <wayround_i2p/ccutils/ip/ip.hpp>

namespace wayround_i2p::ccutils::ip
{

/* // ends at line 455

std::shared_ptr<IP>
    IP::create()
{
    auto ret = std::shared_ptr<IP>(new IP());
    return ret;
}

std::tuple<std::shared_ptr<IP>, error_ptr>
    IP::createFromArray(std::array<std::uint8_t, 4> arr)
{
    auto ret = std::shared_ptr<IP>(new IP());

    auto err = ret->setFromArray(arr);
    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

std::tuple<std::shared_ptr<IP>, error_ptr>
    IP::createFromArray(std::array<std::uint8_t, 16> arr)
{
    auto ret = std::shared_ptr<IP>(new IP());

    auto err = ret->setFromArray(arr);
    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

std::tuple<std::shared_ptr<IP>, error_ptr>
    IP::createFromVector(std::vector<std::uint8_t> vec)
{
    switch (vec.size())
    {
        default:
            return {
                nullptr,
                wayround_i2p::ccutils::errors::New("invalid vector size")
            };
        case 0:
            return {create(), nullptr};
        case 4:
        {
            std::array<std::uint8_t, 4> arr;
            ::memcpy(arr.data(), vec.data(), 4);
            return createFromArray(arr);
        }
        case 16:
        {
            std::array<std::uint8_t, 16> arr;
            ::memcpy(arr.data(), vec.data(), 16);
            return createFromArray(arr);
        }
    }
}

std::tuple<std::shared_ptr<IP>, error_ptr>
    IP::createFromString(UString text)
{
    auto ret = std::shared_ptr<IP>(new IP());

    auto err = ret->setFromString(text);
    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

error_ptr
    IP::setFromArray(std::array<std::uint8_t, 4> arr)
{
    buff.resize(4);
    ::memcpy(buff.data(), arr.data(), 4);
    return nullptr;
}

error_ptr
    IP::setFromArray(std::array<std::uint8_t, 16> arr)
{
    buff.resize(16);
    ::memcpy(buff.data(), arr.data(), 16);
    return nullptr;
}

error_ptr
    IP::setFromArray(std::array<std::uint16_t, 8> arr)
{
    buff.resize(16);
    ::memcpy(buff.data(), arr.data(), 16);
    return nullptr;
}

error_ptr
    IP::setFromVector(std::vector<std::uint8_t> vec)
{
    switch (vec.size())
    {
        default:
            return wayround_i2p::ccutils::errors::New("invalid vector size");
        case 0:
            buff.resize(0);
            return nullptr;
        case 4:
        case 16:
            buff = vec;
            return nullptr;
    }
}

std::tuple<
    error_ptr,

    >
    parseString(UString text)
{
    // todo: todo (requires unicode regexps support in ccutils)
    return {
        wayround_i2p::ccutils::errors::New(
            "IP::setFromString not implemented"
        )
    };
}

bool IP::isSet()
{
    return buff.size() != 0;
}

std::tuple<int, error_ptr>
    IP::getSize()
{
    if (!isSet())
    {
        return {
            0,
            wayround_i2p::ccutils::errors::New("not set")
        };
    }
    return {buff.size(), nullptr};
}

std::tuple<int, error_ptr>
    IP::getVer()
{
    if (!isSet())
    {
        return {
            0,
            wayround_i2p::ccutils::errors::New("not set")
        };
    }

    switch (buff.size())
    {
        case 4:
            return {4, nullptr};
        case 16:
            return {6, nullptr};
    }

    return {
        0,
        wayround_i2p::ccutils::errors::New("programming error")
    };
}

std::tuple<UString, error_ptr>
    IP::toString()
{

    auto ver = getVer();
    if (std::get<1>(ver) != nullptr)
    {
        return {nullptr, std::get<1>(ver)};
    }

    switch (std::get<0>(ver))
    {
        default:
            return {
                nullptr,
                wayround_i2p::ccutils::errors::New("invalid ip version")
            };
        case 4:
            return to4String();
        case 6:
            return to6String();
    }
}

std::tuple<UString, error_ptr>
    IP::to4String()
{

    auto ver = getVer();
    if (std::get<1>(ver) != nullptr)
    {
        return {nullptr, std::get<1>(ver)};
    }

    if (std::get<0>(ver) != 4)
    {
        return {
            nullptr,
            wayround_i2p::ccutils::errors::New("invalid ip version")
        };
    }

    auto ret = UString(
        std::format(
            "%d.%d.%d.%d",
            buff[0],
            buff[1],
            buff[2],
            buff[3]
        )
    );

    return {ret, nullptr};
}

std::tuple<UString, error_ptr>
    IP::to6String()
{

    auto ver = getVer();
    if (std::get<1>(ver) != nullptr)
    {
        return {nullptr, std::get<1>(ver)};
    }

    if (std::get<0>(ver) != 6)
    {
        return {
            nullptr,
            wayround_i2p::ccutils::errors::New("invalid ip version")
        };
    }

    auto ret = UString(
        std::format(
            "%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d",
            buff[0],
            buff[1],
            buff[2],
            buff[3],
            buff[4],
            buff[5],
            buff[6],
            buff[7],
            buff[8],
            buff[9],
            buff[10],
            buff[11],
            buff[12],
            buff[13],
            buff[14],
            buff[15]
        )
    );

    return {ret, nullptr};
}

IP::IP()
{
    buff.resize(0);
}

IP::~IP()
{
}

std::shared_ptr<Port>
    Port::create()
{
    auto ret = std::shared_ptr<Port>(new Port());
    return ret;
}

std::tuple<std::shared_ptr<Port>, error_ptr>
    Port::createFromUInt16(std::uint16_t port)
{
    auto ret = std::shared_ptr<Port>(new Port());
    auto err = ret->setFromUInt16(port);
    if (err)
    {
        return {0, err};
    }
    return {ret, nullptr};
}

std::tuple<std::shared_ptr<Port>, error_ptr>
    Port::createFromString(UString text)
{
    auto ret = std::shared_ptr<Port>(new Port());
    auto err = ret->setFromString(text);
    if (err)
    {
        return {0, err};
    }
    return {ret, nullptr};
}

error_ptr
    Port::setFromUInt16(std::uint16_t port)
{
    this->port = port;
    return nullptr;
}

error_ptr
    Port::setFromString(UString text)
{
    // todo: todo
    return {
        wayround_i2p::ccutils::errors::New(
            "Port::setFromString not implemented"
        )
    };
}

std::tuple<UString, error_ptr>
    Port::toString()
{
    // todo: do better
    return {std::format("{}", port), nullptr};
}

Port::Port()
{
}

Port::~Port()
{
}

std::shared_ptr<IPAndPort>
    IPAndPort::create()
{
    auto ret = std::shared_ptr<IPAndPort>(new IPAndPort());
    return ret;
}

error_ptr
    IPAndPort::setIP(std::shared_ptr<IP> ip)
{
    // todo: input checks?
    this->ip = ip;
    return nullptr;
}

error_ptr
    IPAndPort::setPort(std::shared_ptr<Port> port)
{
    // todo: input checks?
    this->port = port;
    return nullptr;
}

std::shared_ptr<IP>
    IPAndPort::getIP()
{
    return ip;
}

std::shared_ptr<Port>
    IPAndPort::getPort()
{
    return port;
}

void IPAndPort::delIP()
{
    ip = nullptr;
}
void IPAndPort::delPort()
{
    port = nullptr;
}

std::tuple<UString, error_ptr>
    IPAndPort::toString()
{
    auto ip   = getIP();
    auto port = getPort();

    UString ret;

    if (port)
    {
        auto port_str_tup = port->toString();
        if (std::get<1>(port_str_tup))
        {
            return {"", std::get<1>(port_str_tup)};
        }

        ret += std::format(
            ":{}",
            std::get<0>(port_str_tup).to_string()
        );
    }

    if (ip)
    {
        auto ip_str_tup = ip->toString();
        if (std::get<1>(ip_str_tup))
        {
            return {"", std::get<1>(ip_str_tup)};
        }

        auto ip_str = std::get<0>(ip_str_tup);

        auto ver = ip->getVer();
        if (std::get<1>(ver))
        {
            return {"", std::get<1>(ver)};
        }

        if (std::get<0>(ver) == 6)
        {
            ip_str = std::format("[{}]", ip_str.to_string());
        }

        ret = ip_str + ret;
    }

    return {ret, nullptr};
}

IPAndPort::IPAndPort()
{
}

IPAndPort::~IPAndPort()
{
}

*/

// ------------------------ vv new vv ------------------------

IPv4_shared IPv4::create()
{
    auto ret     = IPv4_shared(new IPv4());
    ret->own_ptr = ret;
    return ret;
}

IPv4_shared IPv4::createFromArray(const std::array<std::uint8_t, 4> &arr)
{
    auto ret = IPv4::create();
    ret->setFromArray(arr);
    return ret;
}

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

void IPv4::setFromArray(const std::array<std::uint8_t, 4> &arr)
{
    buff = arr;
}

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

    for (std::size_t i = 0; i != 4; i++)
    {
        buff[i] = vec[i];
    }

    return nullptr;
}

error_ptr IPv4::setFromString(const UString &val)
{
    auto pat = IPv4_STR_PATTERN();
    auto res = pat->match(val);

    if (res->error)
    {
        return res->error;
    }

    if (!res->matched)
    {
        return wayround_i2p::ccutils::errors::New(
            "no match",
            __FILE__,
            __LINE__
        );
    }

    if (res->match_end != val.length())
    {
        return wayround_i2p::ccutils::errors::New(
            "no match",
            __FILE__,
            __LINE__
        );
    }

    std::array<std::uint8_t, 4> tmp;

    for (unsigned char i = 0; i < 4; i++)
    {
        auto res2 = res->searchSubmatchByPatternName(std::to_string(i + 1));

        if (!res2)
        {
            return wayround_i2p::ccutils::errors::New(
                "no match",
                __FILE__,
                __LINE__
            );
        }

        if (res2->error)
        {
            return res2->error;
        }

        if (!res2->matched)
        {
            return wayround_i2p::ccutils::errors::New(
                "no match",
                __FILE__,
                __LINE__
            );
        }

        auto x = res2->getMatchedString();
        try
        {
            tmp[i] = std::stoi(x);
        }
        catch (std::invalid_argument const &ex)
        {
            return wayround_i2p::ccutils::errors::New(
                "invalid_argument",
                __FILE__,
                __LINE__
            );
        }
        catch (std::out_of_range const &ex)
        {
            return wayround_i2p::ccutils::errors::New(
                "out_of_range",
                __FILE__,
                __LINE__
            );
        }
    }
    setFromArray(tmp);
    return nullptr;
}

UString IPv4::toString() const
{
    return std::format("{}.{}.{}.{}", buff[0], buff[1], buff[2], buff[3]);
}

std::array<std::uint8_t, 4> IPv4::toArray() const
{
    return buff;
}

std::vector<std::uint8_t> IPv4::toVector() const
{
    std::vector<std::uint8_t> ret(4);
    for (std::size_t i = 0; i != 4; i++)
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

IPv6_shared IPv6::createFromArray(const IPv6_array &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}

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

void IPv6::setFromArray(const std::array<std::uint16_t, 8> &arr)
{
    buff.b16 = arr;
}

void IPv6::setFromArray(const std::array<std::uint32_t, 4> &arr)
{
    buff.b32 = arr;
}

void IPv6::setFromArray(const IPv6_array &arr)
{
    buff = arr;
}

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

    for (std::size_t i = 0; i != 16; i++)
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

    for (std::size_t i = 0; i != 8; i++)
    {
        buff.b16[i] = vec[i];
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

    for (std::size_t i = 0; i != 4; i++)
    {
        buff.b32[i] = vec[i];
    }

    return nullptr;
}

error_ptr IPv6::setFromString(const UString &text)
{
    std::array<std::uint16_t, 8> tmp;

    auto pat = IPv6_STR_PATTERN();
    auto res = pat->match(text);

    if (res->error)
    {
        return res->error;
    }

    if (!res->matched)
    {
        return wayround_i2p::ccutils::errors::New(
            "no match",
            __FILE__,
            __LINE__
        );
    }

    if (res->match_end != text.length())
    {
        return wayround_i2p::ccutils::errors::New(
            "no match",
            __FILE__,
            __LINE__
        );
    }

    for (auto i : {
             "IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN",
             "IPv6_SHORT_GRP_HEX_STR_PATTERN"
         })
    {
        auto res2 = res->searchSubmatchByPatternName(i);
        if (!res2)
        {
            continue;
        }

        if (res2->error)
        {
            continue;
        }

        if (!res2->matched)
        {
            continue;
        }

        if (res2->match_end != text.length())
        {
            continue;
        }

        if (i == "IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN")
        {
        }

        if (i == "IPv6_SHORT_GRP_HEX_STR_PATTERN")
        {
        }

        return wayround_i2p::ccutils::errors::New(
            "parsing error: IPv6_SHORT_GRP_HEX_STR_PATTERN"
            "|IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN didn't match",
            __FILE__,
            __LINE__
        );
    }

    setFromArray(tmp);
    return nullptr;
}

IPv6_array IPv6::toArray() const
{
    return buff;
}

std::vector<std::uint8_t> IPv6::toVector8() const
{
    std::vector<std::uint8_t> ret(16);
    for (std::size_t i = 0; i < 16; i++)
    {
        ret[i] = buff.b8[i];
    }
    return ret;
}

std::vector<std::uint16_t> IPv6::toVector16() const
{
    std::vector<std::uint16_t> ret(8);
    for (std::size_t i = 0; i < 8; i++)
    {
        ret[i] = buff.b16[i];
    }
    return ret;
}

std::vector<std::uint32_t> IPv6::toVector32() const
{
    std::vector<std::uint32_t> ret(4);
    for (std::size_t i = 0; i < 4; i++)
    {
        ret[i] = buff.b32[i];
    }
    return ret;
}

UString IPv6::toString() const
{
    return toStringShort();
}

UString IPv6::toStringLong() const
{
    UString ret;
    for (std::size_t i = 0; i < 8; i++)
    {
        ret += std::format("{:x}", buff.b16[i]);
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

        // todo: check this is correct in different endianneses

        for (std::size_t i = 0; i < 8; i++)
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

    // todo: check this is correct in different endianneses

    {
        for (
            std::size_t i = 0;
            i < longest_np.start;
            i++
        )
        {
            ret += std::vformat("{:x}", std::make_format_args(buff.b16[i]));
            ret += ":";
        }

        for (
            std::size_t i = longest_np.start + longest_np.length;
            i < 8;
            i++
        )
        {
            ret += ":";
            ret += std::vformat("{:x}", std::make_format_args(buff.b16[i]));
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
    for (std::size_t i = 0; i != 4; i++)
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
    for (std::size_t i = 0; i != 4; i++)
    {
        arr[i] = buff.b8[i + 12];
    }
    return IPv4::createFromArray(arr);
}

IPv6::IPv6()
{
}

IPv6::~IPv6()
{
}

} // namespace wayround_i2p::ccutils::ip
