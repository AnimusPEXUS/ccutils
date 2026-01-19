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

    auto cfa_res = LocalUnixConn::create_for_accepted_FDCtl(res.fdctl);

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

static std::tuple<LocalUnixConn_ptr, error_ptr>
    LocalUnixConn::create(
        ustring      network,
        UnixAddr_ptr laddr
    )
{
    error_ptr err;

    auto ret = LocalUnixConn_ptr(
        new LocalUnixConn(
            // "unix" /*check this*. todo: is this needed at all? use laddr.Net? /,
            laddr.Net,
            laddr,
            err
        )
    );

    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

std::tuple<LocalUnixConn_ptr, error_ptr>
    LocalUnixConn::create_for_accepted_FDCtl(
        wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
    )
{
}

LocalUnixConn::LocalUnixConn(
    ustring      network,
    UnixAddr_ptr laddr
)
{
}

LocalUnixConn::~LocalUnixConn()
{
}

} // namespace wayround_i2p::akigo::net
