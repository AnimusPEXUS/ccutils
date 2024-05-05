

#include <wayround_i2p/ccutils/akigo/net.linux-gnu.hpp>

namespace wayround_i2p::akigo::net
{

std::tuple<UnixListener_ptr, error_ptr> ListenUnix(
    std::string  network,
    UnixAddr_ptr laddr
)
{
    if (network != "unix")
    {
        return std::tuple(
            nullptr,
            wayround_i2p::akigo::errors::New("invalid 'network'")
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
            wayround_i2p::akigo::errors::New("couldn't create UNIX socket")
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

void PosixFDCtlNetConnAdaptor::setFDCtl(std::shared_ptr<FDCtl> fd)
{
    this->fd = fd;
}

std::shared_ptr<FDCtl> PosixFDCtlNetConnAdaptor::getFDCtl()
{
    return fd;
}

error_ptr PosixFDCtlNetConnAdaptor::Close()
{
    if (fd)
    {
        fd->Close();
    }
    // todo: better error check?
    return nullptr;
}

Addr_ptr PosixFDCtlNetConnAdaptor::LocalAddr()
{
}

Addr_ptr PosixFDCtlNetConnAdaptor::RemoteAddr()
{
}

// ------- ↑ ↑ ------- source to be separated ------- ↑ ↑ -------

} // namespace wayround_i2p::akigo::net
