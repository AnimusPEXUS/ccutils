#include <memory>

#include "net.hpp"
#include "net_unix.any.linux-gnu.hpp"

namespace wayround_i2p::akigo::net
{

std::tuple<LocalUnixListener_ptr, error_ptr>
    LocalUnixListener::create(
        ustring      network,
        UnixAddr_ptr laddr
    )
{
    error_ptr err;
    auto      ret = LocalUnixListener_ptr(
        new LocalUnixListener(
            network,
            laddr,
            err
        )
    );
    ret->own_ptr = ret;
    return std::tuple(ret, err);
}

LocalUnixListener::LocalUnixListener(
    ustring      network,
    UnixAddr_ptr laddr,
    error_ptr   &err
)
{
    auto naddr = wayround_i2p::ccutils::posix_tools::FDAddress::create();
    naddr->setUnixAddress(laddr->Name);

    fdctl = wayround_i2p::ccutils::posix_tools::FDCtl::create();
    fdctl->Socket(AF_UNIX, SOCK_STREAM /* todo: SOCK_STREAM here needs evaluation */, 0);
    fdctl->Bind(naddr);
    fdctl->Listen();
}

LocalUnixListener::~LocalUnixListener()
{
}

std::tuple<Conn_ptr, error_ptr>
    LocalUnixListener::Accept()
{
    wayround_i2p::ccutils::posix_tools::FDCtl_FDAddress_res_errNoS res = fdctl->Accept();
    // todo: wrap FDCtl with Conn and return
    return std::tuple(NULL, NULL);
}

std::tuple<UnixConn_ptr, error_ptr>
    LocalUnixListener::AcceptUnix()
{
    FDCtl_FDAddress_res_errNoS res = fdctl->Accept();
    // todo: wrap FDCtl with UnixConn and return
    return std::tuple(NULL, NULL);
}

std::tuple<wayround_i2p::akigo::net::Addr, error_ptr>
    LocalUnixListener::Addr()
{
    wayround_i2p::ccutils::posix_tools::FDAddress_err_errNoS
        res
        = fdctl->GetSockName();
    if (!res || (res && !res->is_ok()))
    {
        return tuple(NULL, "couldn't get unix socket local address");
    }

    auto ua = std::shared_ptr<UnixAddr>(new UnixAddr());

    ustring ua;
    int     ua_name_res;

    std::tie(ua, ua_name_res) = res->addr->getUnixAddress();

    // todo: check ua->Net is correct?

    // todo: maybe better soulution should be found,
    //       because old unix-socket functions does not support long file paths

    return;
}

error_ptr
    LocalUnixListener::Close()
{
}

std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr>
    LocalUnixListener::File()
{
}

error_ptr
    LocalUnixListener::SetDeadline(wayround_i2p::akigo::time::Time t)
{
}

void SetUnlinkOnClose(bool unlink)
{
}

} // namespace wayround_i2p::akigo::net
