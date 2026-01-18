
#include "net_unix.hpp"

namespace wayround_i2p::akigo::net
{

ustring UnixAddr::Network()
{
    return Net;
}

ustring UnixAddr::String()
{
    return Name;
}

std::tuple<UnixListener_ptr, error_ptr> ListenUnix(ustring network, UnixAddr_ptr laddr)
{
    return LocalUnixListener::create(network, laddr);
}

} // namespace wayround_i2p::akigo::net
