

#include <wayround_i2p/ccutils/akigo/net.linux-gnu.hpp>

namespace wayround_i2p::akigo::net
{

std::tuple<UnixListener_ptr, error_ptr> ListenUnix(
    ustring      network,
    UnixAddr_ptr laddr
)
{
    if (network != "unix")
    {
        return std::tuple(
            nullptr,
            wayround_i2p::akigo::errors::New(
                "invalid 'network'",
                __FILE__,
                __LINE__
            )
        );
    }

    auto fdctl = wayround_i2p::ccutils::posix_tools::FDCtl::create(
        0, {.is_open = false}
    );

    auto res = fdctl->Socket(AF_UNIX, SOCK_STREAM, 0);
    if (res.not_ok())
    {
        return std::tuple(
            nullptr,
            wayround_i2p::akigo::errors::New(
                "couldn't create UNIX socket",
                __FILE__,
                __LINE__
            )
        );
    }

    auto ret = PosixFDCtlNetConnAdaptor::create();

    ret->setFDCtl(fdctl);

    return std::tuple(std::dynamic_pointer_cast<UnixListener>(ret), nullptr);
}

// ------- ↓ ↓ ------- source to be separated ------- ↓ ↓ -------

std::shared_ptr<PosixFDCtlNetConnAdaptor> PosixFDCtlNetConnAdaptor::create()
{
    auto ret
        = std::shared_ptr<PosixFDCtlNetConnAdaptor>(new PosixFDCtlNetConnAdaptor());

    ret->own_ptr = ret;

    return ret;
}

PosixFDCtlNetConnAdaptor::PosixFDCtlNetConnAdaptor()
{
}

PosixFDCtlNetConnAdaptor::~PosixFDCtlNetConnAdaptor()
{
}

void PosixFDCtlNetConnAdaptor::setFDCtl(std::shared_ptr<FDCtl> fdctl)
{
    this->fdctl = fdctl;
}

std::shared_ptr<FDCtl> PosixFDCtlNetConnAdaptor::getFDCtl()
{
    return fdctl;
}

error_ptr PosixFDCtlNetConnAdaptor::Close()
{
    if (fdctl)
    {
        fdctl->Close();
    }
    // todo: better error check?
    return nullptr;
}

Addr_ptr PosixFDCtlNetConnAdaptor::LocalAddr()
{
    auto addr = PosixFDCtlNetConnAdaptorAddr::create_for_local(fdctl);
    if (std::get<1>(addr) != 0)
    {
        return nullptr;
    }
    return std::dynamic_pointer_cast<wayround_i2p::akigo::net::Addr>(
        std::get<0>(addr)
    );
}

Addr_ptr PosixFDCtlNetConnAdaptor::RemoteAddr()
{
    auto addr = PosixFDCtlNetConnAdaptorAddr::create_for_peer(fdctl);
    if (std::get<1>(addr) != 0)
    {
        return nullptr;
    }
    return std::dynamic_pointer_cast<wayround_i2p::akigo::net::Addr>(
        std::get<0>(addr)
    );
}

bool PosixFDCtlNetConnAdaptor::SupportsNonBlocking()
{
    return true;
}

bool PosixFDCtlNetConnAdaptor::GetNonBlocking()
{
    bool ret;

    fdctl->isNonBlocking(ret);

    return ret;
}

void PosixFDCtlNetConnAdaptor::SetNonBlocking(bool value)
{
    fdctl->setNonBlocking(value);
}

error_ptr PosixFDCtlNetConnAdaptor::SetDeadline(wayround_i2p::akigo::time::Time t)
{
    // todo: todo
    return nullptr;
}

error_ptr PosixFDCtlNetConnAdaptor::SetReadDeadline(wayround_i2p::akigo::time::Time t)
{
    // todo: todo
    return nullptr;
}

error_ptr PosixFDCtlNetConnAdaptor::SetWriteDeadline(wayround_i2p::akigo::time::Time t)
{
    // todo: todo
    return nullptr;
}

std::tuple<size_type, error_ptr> PosixFDCtlNetConnAdaptor::Read(byte_vector b)
{
    return std::tuple<size_type, error_ptr>();
}

std::tuple<size_type, error_ptr> PosixFDCtlNetConnAdaptor::Write(byte_vector b)
{
    return std::tuple<size_type, error_ptr>();
}

std::tuple<size_type, Addr_ptr, error_ptr> PosixFDCtlNetConnAdaptor::ReadFrom(byte_vector b)
{
    // todo: todo
    return std::tuple<size_type, Addr_ptr, error_ptr>();
}

std::tuple<size_type, error_ptr> PosixFDCtlNetConnAdaptor::WriteTo(byte_vector b, Addr_ptr addr)
{
    // todo: todo
    return std::tuple<size_type, error_ptr>();
}

std::tuple<
    std::shared_ptr<PosixFDCtlNetConnAdaptorAddr>,
    int> // todo: use akigo::error ?
    PosixFDCtlNetConnAdaptorAddr::create_for_local(
        std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl
    )
{
    return PosixFDCtlNetConnAdaptorAddr::create_for_x(fdctl, false);
}

std::tuple<
    std::shared_ptr<PosixFDCtlNetConnAdaptorAddr>,
    int> // todo: use akigo::error ?
    PosixFDCtlNetConnAdaptorAddr::create_for_peer(
        std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl
    )
{
    return PosixFDCtlNetConnAdaptorAddr::create_for_x(fdctl, true);
}

std::tuple<std::shared_ptr<PosixFDCtlNetConnAdaptorAddr>,
           int> // todo: use akigo::error ?
    PosixFDCtlNetConnAdaptorAddr::create_for_x(
        std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl,
        bool                                                       remote
    )
{
    auto ret
        = std::shared_ptr<PosixFDCtlNetConnAdaptorAddr>(
            new PosixFDCtlNetConnAdaptorAddr()
        );

    ret->own_ptr = ret;

    wayround_i2p::ccutils::posix_tools::FDAddress_err_errNoS addr;

    if (!remote)
    {
        addr = fdctl->GetSockName();
    }
    else
    {
        addr = fdctl->GetPeerName();
    }

    if (addr.not_ok())
    {
        return {nullptr, 10};
    }

    auto addr_family = addr.addr->getFamily();
    if (std::get<1>(addr_family) != 0)
    {
        return {nullptr, 20};
    }

    auto sock_type = fdctl->getType();
    if (sock_type.not_ok())
    {
        return {nullptr, 40};
    }

    auto sock_type_int = sock_type.type;

    switch (std::get<0>(addr_family))
    {
        default:
            return {nullptr, 30};

        case AF_UNIX:
        {
            // todo: move this to FDAddress.hpp or somewhere close to it

            auto addr2 = addr.addr->getUnixAddress();
            if (std::get<1>(addr2) != 0)
            {
                return {nullptr, 30};
            }

            {
                wayround_i2p::ccutils::unicode::UString filename
                    = std::get<0>(addr2);

                ret->_string = filename;
            }

            switch (sock_type_int)
            {
                default:
                    return {nullptr, 30};
                case SOCK_STREAM:
                    ret->_network = "unix";
                    break;
                case SOCK_DGRAM:
                    ret->_network = "unixgram";
                    break;
            }

            break;
        }
        case AF_INET:
        {
            // todo: move this to FDAddress.hpp or somewhere close to it

            auto addr2 = addr.addr->getInetAddress();
            if (std::get<2>(addr2) != 0)
            {
                return {nullptr, 30};
            }

            {
                std::array<std::uint8_t, 4> ip; // = std::get<0>(addr2);
                ::memcpy(ip.data(), std::get<0>(addr2).data(), ip.size());
                in_port_t port = std::get<1>(addr2);

                ret->_string = std::format(
                    "%d.%d.%d.%d:%d",
                    ip[0],
                    ip[1],
                    ip[2],
                    ip[3],
                    port
                );
            }

            switch (sock_type_int)
            {
                default:
                    return {nullptr, 30};
                case SOCK_STREAM:
                    ret->_network = "tcp4";
                    break;
                case SOCK_DGRAM:
                    ret->_network = "udp4";
                    break;
            }

            break;
        }
        case AF_INET6:
        {
            // todo: move this to FDAddress.hpp or somewhere close to it

            auto addr2 = addr.addr->getInet6Address();
            if (std::get<2>(addr2) != 0)
            {
                return {nullptr, 30};
            }

            {
                std::array<std::uint8_t, 16> ip; // = std::get<0>(addr2);
                ::memcpy(ip.data(), std::get<0>(addr2).data(), ip.size());
                in_port_t port = std::get<1>(addr2);

                ret->_string = std::format(
                    "[%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d]:%d",
                    ip[0],
                    ip[1],
                    ip[2],
                    ip[3],
                    ip[4],
                    ip[5],
                    ip[6],
                    ip[7],
                    ip[8],
                    ip[9],
                    ip[10],
                    ip[11],
                    ip[12],
                    ip[13],
                    ip[14],
                    ip[15],
                    ip[16],
                    port
                );
            }

            switch (sock_type_int)
            {
                default:
                    return {nullptr, 30};
                case SOCK_STREAM:
                    ret->_network = "tcp6";
                    break;
                case SOCK_DGRAM:
                    ret->_network = "udp6";
                    break;
            }

            break;
        }
    }

    return std::tuple(ret, 0);
}

PosixFDCtlNetConnAdaptorAddr::PosixFDCtlNetConnAdaptorAddr()
{
}

PosixFDCtlNetConnAdaptorAddr::~PosixFDCtlNetConnAdaptorAddr()
{
}

ustring PosixFDCtlNetConnAdaptorAddr::Network()
{
    return _network;
}

ustring PosixFDCtlNetConnAdaptorAddr::String()
{
    return _string;
}

// ------- ↑ ↑ ------- source to be separated ------- ↑ ↑ -------

} // namespace wayround_i2p::akigo::net
