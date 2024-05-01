
#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

#include <wayround_i2p/ccutils/akigo/net.hpp>

namespace wayround_i2p::akigo::net
{

std::tuple<UnixListener_ptr, error_ptr> ListenUnix(
    std::string  network,
    UnixAddr_ptr laddr
)
{
    if (network == "unix")
    {
        auto fdctl = wayround_i2p::ccutils::posix_tools::FDCtl::create(0);
        fdctl->Socket();
    }
}

} // namespace wayround_i2p::akigo::net
