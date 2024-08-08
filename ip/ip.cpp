
#include <wayround_i2p/ccutils/ip/ip.hpp>

namespace wayround_i2p::ccutils::ip
{

constexpr regexp::Pattern_shared IPv4_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
              {regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("1"),
               regexp::Pattern::newExactChar("[")
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("2"),
               regexp::Pattern::newExactChar("[")
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("3"),
               regexp::Pattern::newExactChar("[")
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("4")
              }
        )
              ->setName("IPv4_STR_PATTERN");
    return ret;
}

constexpr regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
              {regexp::Pattern::newSequence(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(1, 4)
                        ->setName("num"),
                    regexp::Pattern::newExactChar(":")
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
              ->setName("IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN");
    return ret;
}

constexpr regexp::Pattern_shared IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
              {regexp::Pattern::newSequence(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(1, 2)
                        ->setName("num"),
                    regexp::Pattern::newExactChar(":")
                        ->setMinMaxCount(1, 1)}
               )
                   ->setMinMaxCount(15, 15)
                   ->setName("numbers"),
               regexp::Pattern::newCharIsXDigit()
                   ->setMinMaxCount(1, 2)
                   ->setName("15")}
        )
              ->setName("IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN");
    return ret;
}

constexpr regexp::Pattern_shared IP_STR_PATTERN()
{
    regexp::Pattern_shared ret
        = regexp::Pattern::newSequence(
              {regexp::Pattern::newOrGroup(
                   {/* ipv4 pattern */
                    IPv4_STR_PATTERN(),
                    /* two ipv6 patterns */
                    regexp::Pattern::newSequence(
                        {regexp::Pattern::newExactChar("[")->setMaxCount(1),
                         regexp::Pattern::newOrGroup(
                             {/* long 1 byte */
                              IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN(),
                              /* long 2 byte */
                              IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN(),
                              /* short */
                              regexp::Pattern::newSequence(
                                  {regexp::Pattern::newAnyChar(),
                                   regexp::Pattern::newExactChar(":")
                                       ->setExactCount(2),
                                   regexp::Pattern::newAnyChar()
                                  }
                              )
                             }
                         ),
                         regexp::Pattern::newExactChar("]")
                             ->setMaxCount(1)
                        }
                    )
                   }
               )
                   ->setName("ip"),
               regexp::Pattern::newSequence(
                   {regexp::Pattern::newExactChar(":"),
                    regexp::Pattern::newCharIsDigit()
                        ->setName("port")
                        ->setMinCount(1)
                   }
               )
              }
        )
              ->setName("ip_and_port");

    return ret;
}

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

error_ptr
    IP::setFromString(UString text)
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

} // namespace wayround_i2p::ccutils::ip
