
#include <wayround_i2p/ccutils/posix_tools/FDAddress.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

std::shared_ptr<FDAddress> FDAddress::create()
{
    auto ret     = std::shared_ptr<FDAddress>(new FDAddress());
    ret->own_ptr = ret;
    return ret;
}

static std::shared_ptr<FDAddress> FDAddress::create(
    std::vector<std::uint8_t> addr_buff
)
{
    auto ret     = std::shared_ptr<FDAddress>(new FDAddress(addr_buff));
    ret->own_ptr = ret;
    return ret;
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

int FDAddress::setAddressFromFDCtl(std::shared_ptr<FDCtl> fd)
{
    if (!fd)
    {
        return -4;
    }
    return fd->setFDAddr(own_ptr.lock());
}

int FDAddress::setAddrBuff(std::vector<std::uint8_t> addr_buff)
{
    if (remove_fdctl_binding)
    {
        fd.reset();
    }
    this->addr_buff = addr_buff;
}

std::tuple<sa_family_t, int> FDAddress::getFamily()
{
    sa_family_t ret_sa_family_t;

    if (addr_buff.size() < sizeof(sa_family_t))
    {
        return tuple(0, -1);
    }

    ret_sa_family_t = *(cast_reinterprete<sa_family_t *>(addr_buff.data));

    return tuple(ret_sa_family_t, 0);
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

int setAddress(std::string text)
{
    // todo: todo
    return -10;
}

int setAddress(unsigned char addr[4], in_port_t port)
{
    return setAddress(cast_reinterprete<uint32_t>(addr), port)
}

int setAddress(std::uint8_t addr[4], in_port_t port)
{
    return setAddress(cast_reinterprete<uint32_t>(addr), port)
}

int setAddress(uint32_t addr, in_port_t port)
{
    auto addr_st = std::shared_ptr<sockaddr_in>(new sockaddr_in());

    addr_st->sin_family      = AF_INET;
    addr_st->sin_port        = port;
    addr_st->sin_addr.s_addr = addr;
    return setInetAddress(addr_st);
}

int setAddress(unsigned char addr[16])
{
    auto addr_st = std::shared_ptr<sockaddr_in6>(new sockaddr_in6());

    addr_st->sin6_family       = AF_INET6;
    addr_st->sin6_port         = port;
    addr_st->sin6_addr.s6_addr = addr;
    return setInet6Address(addr_st);
}

int setAddress(std::uint8_t addr[16])
{
    return setAddress(cast_reinterprete<unsigned char[16]>(addr))
}

int setAddress(std::vector<std::uint8_t> addr)
{
    auto s = addr.size();

    switch (s)
    {
        default:
            return -1;
        case 4:
            return setAddress(cast_reinterprete<uint32_t>(*(addr.data())));
        case 16:
            return setAddress(cast_reinterprete<unsigned char[16]>(*(addr.data())))
    }
}

int setUnixAddress(std::string text)
{
    auto ustr = icu::UnicodeString(text);
    return setUnixAddress(ustr);
}

int setUnixAddress(icu::UnicodeString text)
{

    auto std::string utf8_str;
    size_t           utf8_str_size = 0;

    utf8_str = text.toUTF8String(utf8_str);

    utf8_str_size = std::strlen(utf8_str);

    if (utf8_str_size >= 107)
    {
        return -2;
    }

    auto addr_st = std::shared_ptr<sockaddr_un>(new sockaddr_un());

    addr_st->sun_family = AF_UNIX;

    std::strcpy(addr_st->sun_path, utf8_str);

    return setUnixAddress(addr_st);
}

int FDAddress::setUnixAddress(std::shared_ptr<sockaddr_un> addr)
{
    if (std::strlen(addr->get()) >= 107)
    {
        return -3;
    }
    addr_buff.assign(sizeof(sockaddr_un), 0);
    return ::memcpy(addr_buff.data(), addr.get(), sizeof(sockaddr_un));
}

int FDAddress::setInetAddress(std::shared_ptr<sockaddr_in> addr)
{
    addr_buff.assign(sizeof(sockaddr_in), 0);
    return ::memcpy(addr_buff.data(), addr.get(), sizeof(sockaddr_in));
}

int FDAddress::setInet6Address(std::shared_ptr<sockaddr_in6> addr)
{
    addr_buff.assign(sizeof(sockaddr_in6), 0);
    return ::memcpy(addr_buff.data(), addr.get(), sizeof(sockaddr_in6));
}

std::tuple<std::shared_ptr<sockaddr_un>, int> getUnixAddress()
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

    ::memcpy(ret.data, add_buff.data, add_buff.data.size());

    return std::tuple(ret, 0);
}

std::tuple<std::shared_ptr<sockaddr_in>, int> getInetAddress()
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

    ::memcpy(ret.data, add_buff.data, add_buff.data.size());

    return std::tuple(ret, 0);
}

std::tuple<std::shared_ptr<sockaddr_in6>, int> getInet6Address()
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

    ::memcpy(ret.data, add_buff.data, add_buff.data.size());

    return std::tuple(ret, 0);
}

} // namespace wayround_i2p::ccutils::posix_tools
