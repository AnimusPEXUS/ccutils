#include <memory>

#include "errors.hpp"
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
    return {ret, err};
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
    fdctl->Socket(
        AF_UNIX,
        SOCK_STREAM /* todo: SOCK_STREAM here needs evaluation */,
        0
    );
    fdctl->Bind(naddr);
    fdctl->Listen();
}

LocalUnixListener::~LocalUnixListener()
{
}

std::tuple<Conn_ptr, error_ptr>
    LocalUnixListener::Accept()
{
    auto res = AcceptUnix();
    auto err = std::get<1>(res);

    if (err)
    {
        return {nullptr, err};
    }

    return {
        std::dynamic_pointer_cast<
            wayround_i2p::akigo::net::Conn>(std::get<0>(res)),
        nullptr
    };
}

std::tuple<UnixConn_ptr, error_ptr>
    LocalUnixListener::AcceptUnix()
{
    auto res = fdctl->Accept();

    if (!res.is_ok())
    {
        return {
            nullptr,
            wayround_i2p::akigo::errors::New(
                std::format(
                    "couldn't accept connection. errno: %d",
                    res.errNo
                ),
                __FILE__,
                __LINE__
            )
        };
    }

    UnixConn_ptr ret;

    auto cfa_res = LocalUnixConn::create_for_FDCtl(res.fdctl);

    auto err = std::get<1>(cfa_res);

    if (err)
    {
        return {nullptr, err};
    }

    ret = std::get<0>(cfa_res);

    return {ret, nullptr};
}

std::tuple<wayround_i2p::akigo::net::Addr_ptr, error_ptr>
    LocalUnixListener::Addr()
{
    auto res = fdctl->GetSockName();
    if (!res.is_ok())
    {
        return {
            nullptr,
            wayround_i2p::akigo::errors::New(
                "couldn't get unix socket local address",
                __FILE__,
                __LINE__
            )
        };
    }

    auto uaddr_res     = res.addr->getUnixAddress();
    auto uaddr_res_err = std::get<1>(uaddr_res);

    if (uaddr_res_err != 0)
    {
        return {
            nullptr,
            wayround_i2p::akigo::errors::New(
                "couldn't get unix socket local address",
                __FILE__,
                __LINE__
            )
        };
    }

    auto ret = UnixAddr_ptr(new UnixAddr());

    ret->Name = std::get<0>(uaddr_res);
    ret->Net  = "unix";

    // todo: check `ret->Net = "unix"` is correct?

    // todo: maybe better soulution should be found,
    //       because old unix-socket functions does not support long file paths

    return {
        std::dynamic_pointer_cast<wayround_i2p::akigo::net::Addr>(ret),
        nullptr
    };
}

error_ptr
    LocalUnixListener::Close()
{
    auto res = fdctl->Close();
    if (!res.is_ok())
    {
        return wayround_i2p::akigo::errors::New(
            "Couldn't close file",
            __FILE__,
            __LINE__
        );
    }
    return nullptr;
}

std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr>
    LocalUnixListener::File()
{
#warning "todo"
    return {
        wayround_i2p::akigo::os::File_ptr(
            new wayround_i2p::akigo::os::File()
        ),
        wayround_i2p::akigo::errors::New(
            "todo",
            __FILE__,
            __LINE__
        )
    };
}

error_ptr
    LocalUnixListener::SetDeadline(wayround_i2p::akigo::time::Time t)
{
#warning "todo"
    return wayround_i2p::akigo::errors::New(
        "todo",
        __FILE__,
        __LINE__
    );
}

void LocalUnixListener::SetUnlinkOnClose(bool unlink)
{
#warning "todo"
}

std::tuple<LocalUnixConn_ptr, error_ptr>
    LocalUnixConn::create_for_FDCtl(
        wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
    )
{
    auto ret = LocalUnixConn_ptr(
        new LocalUnixConn(fdctl)
    );

    ret->own_ptr = ret;

    return ret;
}

LocalUnixConn::LocalUnixConn(
    wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
) : fdctl(fdctl),
    is_open(true)
{
}

LocalUnixConn::~LocalUnixConn()
{
}

std::tuple<go_int, error_ptr> LocalUnixConn::Write(byte_slice p)
{
}

error_ptr LocalUnixConn::Close()
{
    is_open  = false;
    auto res = fdctl->Close();

    auto err = res.err;
    if (err)
    {
        return err;
    }

    return nullptr;
}

std::tuple<go_int, error_ptr> LocalUnixConn::Read(byte_slice p)
{
}

error_ptr LocalUnixConn::SetDeadline(wayround_i2p::akigo::time::Time t)
{
}

error_ptr LocalUnixConn::SetReadDeadline(wayround_i2p::akigo::time::Time t)
{
}

error_ptr LocalUnixConn::SetWriteDeadline(wayround_i2p::akigo::time::Time t)
{
}

std::tuple<go_int64, error_ptr> LocalUnixConn::WriteTo(Writer_ptr w)
{
}

std::tuple<go_int64, error_ptr> LocalUnixConn::ReadFrom(Reader_ptr r)
{
}

error_ptr LocalUnixConn::CloseRead()
{
}

error_ptr LocalUnixConn::CloseWrite()
{
}

error_ptr LocalUnixConn::SetReadBuffer(int bytes)
{
}

error_ptr LocalUnixConn::SetWriteBuffer(int bytes)
{
}

std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> LocalUnixConn::File()
{
}

Addr_ptr LocalUnixConn::LocalAddr()
{
}

Addr_ptr LocalUnixConn::RemoteAddr()
{
}

std::tuple<int, UnixAddr_ptr, error_ptr> LocalUnixConn::ReadFromUnix(byte_slice b)
{
}

std::tuple<
    int,          // n
    int,          // oobn
    int,          // flags
    UnixAddr_ptr, // addr
    error_ptr     // error
    >
    LocalUnixConn::ReadMsgUnix(
        byte_slice b,
        byte_slice oob
    )
{
}

std::tuple<
    int,      // n
    int,      // oobn
    error_ptr // err
    >
    LocalUnixConn::WriteMsgUnix(
        byte_slice   b,
        byte_slice   oob,
        UnixAddr_ptr addr
    )
{
}

std::tuple<int, error_ptr> LocalUnixConn::WriteToUnix(byte_slice b, UnixAddr_ptr addr)
{
}

} // namespace wayround_i2p::akigo::net
