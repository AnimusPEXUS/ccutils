
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

FDAddress::~FDAddress()
{
}

int setAddressFromFDCtl(std::shared_ptr<FDCtl> fd)
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

    switch (family)
    {
        default:
            return tuple("", -1);
        case AF_INET:
            return tuple("inet", 0);
        case AF_INET6:
            return tuple("inet6", 0);
        case AF_UNIX:
            return tuple("unix", 0);
    }
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
