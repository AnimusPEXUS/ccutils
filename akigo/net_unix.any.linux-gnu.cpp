#include <memory>

#include <wayround_i2p/ccutils/akigo/net.hpp>

#include <wayround_i2p/ccutils/akigo/PosixFDCtlNetConnAdapter.hpp>

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

    auto ret = PosixFDCtlNetConnAdapter::create();

    ret->setFDCtl(fdctl);

    std::cout << "ListenUnix, ret: " << (ret == nullptr ? "" : "not") << " null" << std::endl;

    auto x = std::dynamic_pointer_cast<UnixListener>(ret);

    std::cout << "ListenUnix,   x: " << (x == nullptr ? "" : "not") << " null" << std::endl;

    return std::tuple(x, nullptr);
}

} // namespace wayround_i2p::akigo::net
