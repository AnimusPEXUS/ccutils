#ifndef WAYROUND_I2P_20240308_140051_32629
#define WAYROUND_I2P_20240308_140051_32629

#include <memory>
#include <string>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/time.hpp>

namespace wayround_i2p::akigo::net
{
using error       = wayround_i2p::akigo::builtin::error;
using size_type   = wayround_i2p::akigo::builtin::size_type;
using byte_vector = wayround_i2p::akigo::builtin::byte_vector;

class Addr_nsp
{
    virtual std::string Network() = 0;
    virtual std::string String()  = 0;
};

using Addr = std::shared_ptr<Addr_nsp>;

class Conn
{
    virtual std::tuple<size_type, error> Read(byte_vector b)  = 0;
    virtual std::tuple<size_type, error> Write(byte_vector b) = 0;
    virtual error                        Close()              = 0;

    virtual Addr LocalAddr()  = 0;
    virtual Addr RemoteAddr() = 0;

    virtual error SetDeadline(wayround_i2p::akigo::time::Time t)      = 0;
    virtual error SetReadDeadline(wayround_i2p::akigo::time::Time t)  = 0;
    virtual error SetWriteDeadline(wayround_i2p::akigo::time::Time t) = 0;
};

using Conn_ptr = std::shared_ptr<Conn>;

std::tuple<Conn, error> Dial(std::string network, std::string address);
std::tuple<Conn, error> DialTimeout(
    std::string                         network,
    std::string                         address,
    wayround_i2p::akigo::time::Duration timeout
);

class Dialer
{
};

} // namespace wayround_i2p::akigo::net

#endif
