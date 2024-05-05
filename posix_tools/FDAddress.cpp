#include <bit>
#include <cstring>

#include <wayround_i2p/ccutils/posix_tools/FDAddress.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

std::shared_ptr<FDAddress> FDAddress::create()
{
    auto ret     = std::shared_ptr<FDAddress>(new FDAddress());
    ret->own_ptr = ret;
    return ret;
}

std::shared_ptr<FDAddress> FDAddress::create(
    std::vector<std::uint8_t> addr_buff
)
{
    auto ret     = std::shared_ptr<FDAddress>(new FDAddress(addr_buff));
    ret->own_ptr = ret;
    return ret;
}

std::shared_ptr<FDAddress> FDAddress::create(
    struct sockaddr *addr,
    socklen_t        length
)
{
    if (length != sizeof(sockaddr))
    {
        return nullptr;
    }

    std::vector<uint8_t> addr_buff;

    addr_buff.resize(length);

    ::memcpy(addr_buff.data(), addr, length);

    return FDAddress::create(addr_buff);
}

FDAddress::FDAddress()
{
}

FDAddress::FDAddress(std::vector<std::uint8_t> addr_buff)
{
    this->addr_buff = addr_buff;
}

FDAddress::~FDAddress()
{
}

/* // circular dependency :(
err_errNoS FDAddress::setAddressFromFDCtl(std::shared_ptr<FDCtl> fd)
{
    if (!fd)
    {
        return {-4, 0};
    }
    return fd->setFDAddress(own_ptr.lock());
}
*/

int FDAddress::setAddrBuff(std::vector<std::uint8_t> addr_buff)
{
    // note: theoretically here can be check (parameter validation,
    //       or something else), so this function can return non-0
    //       on error
    this->addr_buff = addr_buff;
    return 0;
}

std::vector<std::uint8_t> FDAddress::getAddrBuff()
{
    return addr_buff;
}

std::tuple<sa_family_t, int> FDAddress::getFamily()
{
    sa_family_t ret_sa_family_t;

    if (addr_buff.size() < sizeof(sa_family_t))
    {
        return std::tuple(0, -1);
    }

    ret_sa_family_t = *(reinterpret_cast<sa_family_t *>(addr_buff.data()));

    return std::tuple(ret_sa_family_t, 0);
}

std::tuple<std::string, int> FDAddress::getFamilyString()
{
    auto [family, err] = getFamily();
    if (err != 0)
    {
        return std::tuple("", err);
    }

    switch (family)
    {
        default:
            return std::tuple("", -1);
        case AF_INET:
            return std::tuple("AF_INET", 0);
        case AF_INET6:
            return std::tuple("AF_INET6", 0);
        case AF_UNIX:
            return std::tuple("AF_UNIX", 0);
    }
}

int FDAddress::setInetAddress(
    wayround_i2p::ccutils::unicode::UString text,
    in_port_t                               port
)
{
    // todo: todo
    return -10;
}

int FDAddress::setInetAddress(std::vector<std::uint8_t> addr, in_port_t port)
{
    auto s = addr.size();

    switch (s)
    {
        default:
            return -1;
        case 4:
            return setInetAddress(
                reinterpret_cast<std::uint8_t *>(addr.data()),
                port
            );
        case 16:
            return setInet6Address(
                reinterpret_cast<std::uint8_t *>(addr.data()),
                port
            );
    }
}

int FDAddress::setInetAddress(std::uint8_t addr[4], in_port_t port)
{
    std::uint32_t t;

    if constexpr (std::endian::native == std::endian::big)
    {
        for (int i = 0; i != sizeof(std::uint32_t); i++)
        {
            ((std::uint8_t *)&t)[i] = addr[i];
        }
    }
    else
    {
        for (int i = 0; i != sizeof(std::uint32_t); i++)
        {
            ((std::uint8_t *)&t)[sizeof(std::uint32_t) - 1 - i] = addr[i];
        }
    }

    return setInetAddress(t, port);
}

int FDAddress::setInetAddress(std::uint32_t addr, in_port_t port)
{
    auto addr_st = std::shared_ptr<sockaddr_in>(new sockaddr_in());

    addr_st->sin_family      = AF_INET;
    addr_st->sin_port        = port;
    addr_st->sin_addr.s_addr = addr;
    return setInetAddress(addr_st);
}

int FDAddress::setInet6Address(std::uint8_t addr[16], in_port_t port)
{
    auto addr_st = std::shared_ptr<sockaddr_in6>(new sockaddr_in6());

    for (auto i = 0; i != 16; i++)
    {
        addr_st->sin6_addr.s6_addr[i] = addr[i];
    }

    addr_st->sin6_family = AF_INET6;
    addr_st->sin6_port   = port;
    // addr_st->sin6_addr.s6_addr = addr;
    return setInet6Address(addr_st);
}

int FDAddress::setUnixAddress(wayround_i2p::ccutils::unicode::UString text)
{
    std::string utf8_str;
    size_t      utf8_str_size = 0;

    utf8_str = text.string_utf8();

    utf8_str_size = std::strlen(utf8_str.c_str());

    if (utf8_str_size >= 107)
    {
        return -2;
    }

    auto addr_st = std::shared_ptr<sockaddr_un>(new sockaddr_un());

    addr_st->sun_family = AF_UNIX;

    std::strcpy(addr_st->sun_path, utf8_str.c_str());

    return setUnixAddress(addr_st);
}

int FDAddress::setUnixAddress(std::shared_ptr<sockaddr_un> addr)
{
    if (std::strlen(addr.get()->sun_path) >= 107)
    {
        return -3;
    }
    addr_buff.assign(sizeof(sockaddr_un), 0);
    ::memcpy(addr_buff.data(), addr.get(), sizeof(sockaddr_un));
    return 0;
}

int FDAddress::setInetAddress(std::shared_ptr<sockaddr_in> addr)
{
    addr_buff.assign(sizeof(sockaddr_in), 0);
    ::memcpy(addr_buff.data(), addr.get(), sizeof(sockaddr_in));
    return 0;
}

int FDAddress::setInet6Address(std::shared_ptr<sockaddr_in6> addr)
{
    addr_buff.assign(sizeof(sockaddr_in6), 0);
    ::memcpy(addr_buff.data(), addr.get(), sizeof(sockaddr_in6));
    return 0;
}

std::tuple<std::shared_ptr<sockaddr_un>, int> FDAddress::get_sockaddr_un()
{
    auto [family, err] = getFamily();
    if (err != 0)
    {
        return std::tuple(nullptr, -1);
    }

    if (family != AF_UNIX)
    {
        return std::tuple(nullptr, -2);
    }

    auto ret = std::shared_ptr<sockaddr_un>(new sockaddr_un());

    ::memcpy(ret.get(), addr_buff.data(), addr_buff.size());

    return std::tuple(ret, 0);
}

std::tuple<std::shared_ptr<sockaddr_in>, int> FDAddress::get_sockaddr_in()
{
    auto [family, err] = getFamily();
    if (err != 0)
    {
        return std::tuple(nullptr, -1);
    }

    if (family != AF_INET)
    {
        return std::tuple(nullptr, -2);
    }

    auto ret = std::shared_ptr<sockaddr_in>(new sockaddr_in());

    ::memcpy(ret.get(), addr_buff.data(), addr_buff.size());

    return std::tuple(ret, 0);
}

std::tuple<std::shared_ptr<sockaddr_in6>, int> FDAddress::get_sockaddr_in6()
{
    auto [family, err] = getFamily();
    if (err != 0)
    {
        return std::tuple(nullptr, -1);
    }

    if (family != AF_INET6)
    {
        return std::tuple(nullptr, -2);
    }

    auto ret = std::shared_ptr<sockaddr_in6>(new sockaddr_in6());

    ::memcpy(ret.get(), addr_buff.data(), addr_buff.size());

    return std::tuple(ret, 0);
}

} // namespace wayround_i2p::ccutils::posix_tools
