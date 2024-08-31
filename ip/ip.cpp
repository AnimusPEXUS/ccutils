
#include <wayround_i2p/ccutils/ip/ip.hpp>

namespace wayround_i2p::ccutils::ip
{

/*

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
        return wayround_i2p::ccutils::errors::New("invalid vector size");
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
        return wayround_i2p::ccutils::errors::New("no match");
    }

    if (res->match_end != val.length())
    {
        return wayround_i2p::ccutils::errors::New("no match");
    }

    std::array<std::uint8_t, 4> tmp;

    for (unsigned char i = 0; i != 4; i++)
    {
        auto res2 = res->searchSubmatchByPatternName(std::to_string(i + 1));

        if (!res2)
        {
            return wayround_i2p::ccutils::errors::New("no match");
        }

        if (res2->error)
        {
            return res2->error;
        }

        if (!res2->matched)
        {
            return wayround_i2p::ccutils::errors::New("no match");
        }

        auto x = res2->getMatchedString();
        try
        {
            tmp[i] = std::stoi(x);
        }
        catch (std::invalid_argument const &ex)
        {
            return wayround_i2p::ccutils::errors::New("invalid_argument");
        }
        catch (std::out_of_range const &ex)
        {
            return wayround_i2p::ccutils::errors::New("out_of_range");
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
    buff = arr;
}

void IPv6::setFromArray(const std::array<std::uint16_t, 8> &arr)
{
    for (std::size_t i = 0; i != 16; i++)
    {
        buff[i] = ((std::uint8_t *)arr.data())[i];
    }
}

error_ptr IPv6::setFromVector(const std::vector<std::uint8_t> &vec)
{
    if (vec.size() != 16)
    {
        return wayround_i2p::ccutils::errors::New("invalid vector size");
    }

    for (std::size_t i = 0; i != 16; i++)
    {
        buff[i] = vec[i];
    }
}

error_ptr IPv6::setFromVector(const std::vector<std::uint16_t> &vec)
{
    if (vec.size() != 8)
    {
        return wayround_i2p::ccutils::errors::New("invalid vector size");
    }

    for (std::size_t i = 0; i != 16; i++)
    {
        buff[i] = ((std::uint8_t *)vec.data())[i];
    }
}

error_ptr IPv6::setFromString(const UString &text)
{
    // todo: todo
    return nullptr;
}

UString IPv6::toString(bool long_particle) const
{
    return toStringShort();
}

UString IPv6::toStringLong(bool long_particle) const
{
    UString ret;
    for (std::size_t i = 0; i < 16; i += (long_particle ? 2 : 1))
    {
        ret += std::to_string(buff[i]);
        if (i < 15)
        {
            ret += ":";
        }
    }
    return ret;
}

UString IPv6::toStringShort(bool long_particle) const
{
    struct zeroes_slice
    {
        std::size_t start;
        std::size_t length;
    };

    std::deque<zeroes_slice> slices;

    bool        zeroes_slice_started = false;
    std::size_t zeroes_slice_i       = -1;

    auto search_slices
        = [&]<
              typename ParticleType,
              char particle_count>() -> void
    {
        auto buff_t = (ParticleType *)(buff.data());
        for (std::size_t i = 0; i < particle_count; i++)
        {
            if (buff_t[i] == 0)
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
    };

    if (long_particle)
    {
        search_slices.operator()<std::uint16_t, 8>();
    }
    else
    {
        search_slices.operator()<std::uint8_t, 16>();
    }

    if (slices.size() == 0)
    {
        return toStringLong(long_particle);
    }
#error "continue here"
}

std::array<std::uint8_t, 16> IPv6::toArray() const
{
}

std::vector<std::uint8_t> IPv6::toVector() const
{
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
        buff[i + 12] = arr[i];
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
        arr[i] = buff[i + 12];
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
