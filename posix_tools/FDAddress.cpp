#include <bit>
#include <cstring>

#include <wayround_i2p/ccutils/posix_tools/FDAddress.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

FDAddress_ptr FDAddress::create()
{
    auto ret     = FDAddress_ptr(new FDAddress());
    ret->own_ptr = ret;
    return ret;
}

FDAddress_ptr FDAddress::create(
    std::vector<std::uint8_t> addr_buff
)
{
    auto ret     = FDAddress_ptr(new FDAddress(addr_buff));
    ret->own_ptr = ret;
    return ret;
}

FDAddress_ptr FDAddress::create(
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

    auto data_ptr = addr.data();

    switch (s)
    {
        default:
            return -1;
        case 4:
            return setInetAddress(
                // todo: endianness?
                // todo: better way?
                std::array<std::uint8_t, 4>{
                    data_ptr[0],
                    data_ptr[1],
                    data_ptr[2],
                    data_ptr[3]
                },
                port
            );
        case 16:
            return setInet6Address(
                // todo: endianness?
                // todo: better way?
                std::array<std::uint8_t, 16>{
                    data_ptr[0],
                    data_ptr[1],
                    data_ptr[2],
                    data_ptr[3],
                    data_ptr[4],
                    data_ptr[5],
                    data_ptr[6],
                    data_ptr[7],
                    data_ptr[8],
                    data_ptr[9],
                    data_ptr[10],
                    data_ptr[11],
                    data_ptr[12],
                    data_ptr[13],
                    data_ptr[14],
                    data_ptr[15]
                },
                port
            );
    }
}

int FDAddress::setInetAddress(std::array<std::uint8_t, 4> addr, in_port_t port)
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

int FDAddress::setInet6Address(std::array<std::uint8_t, 16> addr, in_port_t port)
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

    utf8_str = text.to_string();

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

std::tuple<wayround_i2p::ccutils::unicode::UString, int> FDAddress::getUnixAddress()
{
    auto res = get_sockaddr_un();
    auto err = std::get<1>(res);
    if (err != 0)
    {
        return {nullptr, err};
    }

    return {std::get<0>(res)->sun_path, 0};
}

std::tuple<std::array<std::uint8_t, 4>, in_port_t, int> FDAddress::getInetAddress()
{
    auto res     = get_sockaddr_in();
    auto res_err = std::get<1>(res);
    if (res_err != 0)
    {
        return std::tuple<std::array<std::uint8_t, 4>, in_port_t, int>(
            {0, 0, 0, 0},
            0,
            res_err
        );
    }

    std::array<std::uint8_t, 4> ret;

    ::memcpy(ret.data(), &(std::get<0>(res)->sin_addr), 4);

    return {ret, std::get<0>(res)->sin_port, 0};
}

std::tuple<std::array<std::uint8_t, 16>, in_port_t, int> FDAddress::getInet6Address()
{
    auto res     = get_sockaddr_in6();
    auto res_err = std::get<1>(res);
    if (res_err != 0)
    {
        return std::tuple<std::array<std::uint8_t, 16>, in_port_t, int>(
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            0,
            res_err
        );
    }

    std::array<std::uint8_t, 16> ret;

    ::memcpy(ret.data(), &(std::get<0>(res)->sin6_addr), 16);

    return {ret, std::get<0>(res)->sin6_port, 0};
}

std::tuple<std::shared_ptr<sockaddr_un>, int> FDAddress::get_sockaddr_un()
{
    if (addr_buff.size() >= 107)
    {
        return std::tuple(nullptr, -3);
    }

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
