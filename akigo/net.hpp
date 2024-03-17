#ifndef WAYROUND_I2P_20240310_155748_300049
#define WAYROUND_I2P_20240310_155748_300049

#include <memory>
#include <string>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/context.hpp>
#include <wayround_i2p/ccutils/akigo/os.hpp>
#include <wayround_i2p/ccutils/akigo/time.hpp>

namespace wayround_i2p::akigo::net
{
using error_ptr   = wayround_i2p::akigo::builtin::error_ptr;
using size_type   = wayround_i2p::akigo::builtin::size_type;
using byte_vector = wayround_i2p::akigo::builtin::byte_vector;
using Context_ptr = wayround_i2p::akigo::context::Context_ptr;
using File_ptr    = wayround_i2p::akigo::os::File_ptr;

class Error : public error_ptr
{
    bool Timeout();
};

class Addr
{
    virtual std::string Network() = 0;
    virtual std::string String()  = 0;
};

using Addr_ptr = std::shared_ptr<Addr>;
/**
 * WARNING!: in distinction to Go's net.Conn, akigo's [net::Conn]s support
 *           non-blocking states via 3 additional functions
 */
class ConnBase
{
    virtual error_ptr Close() = 0;

    virtual Addr_ptr LocalAddr()  = 0;
    virtual Addr_ptr RemoteAddr() = 0;

    virtual bool SupportsNonBlocking()      = 0;
    virtual bool GetNonBlocking()           = 0;
    virtual void SetNonBlocking(bool value) = 0;

    virtual error_ptr SetDeadline(wayround_i2p::akigo::time::Time t)      = 0;
    virtual error_ptr SetReadDeadline(wayround_i2p::akigo::time::Time t)  = 0;
    virtual error_ptr SetWriteDeadline(wayround_i2p::akigo::time::Time t) = 0;
};

class Conn : public ConnBase
{
    virtual std::tuple<size_type, error_ptr> Read(byte_vector b)  = 0;
    virtual std::tuple<size_type, error_ptr> Write(byte_vector b) = 0;
};

using Conn_ptr = std::shared_ptr<Conn>;

class PacketConn : public ConnBase
{
    virtual std::tuple<size_type, Addr_ptr, error_ptr> ReadFrom(byte_vector b)               = 0;
    virtual std::tuple<size_type, error_ptr>           WriteTo(byte_vector b, Addr_ptr addr) = 0;
};

using PacketConn_ptr = std::shared_ptr<PacketConn>;

std::tuple<Conn_ptr, error_ptr> Dial(
    std::string network,
    std::string address
);

std::tuple<Conn_ptr, error_ptr> DialTimeout(
    std::string                         network,
    std::string                         address,
    wayround_i2p::akigo::time::Duration timeout
);

class Dialer
{
  public:
    wayround_i2p::akigo::time::Duration Timeout;
    wayround_i2p::akigo::time::Time     Deadline;
    Addr_ptr                            LocalAddr;

    std::function<
        error_ptr(
            std::string network,
            int         fd
        )>
        Control;

    std::function<
        error_ptr(
            Context_ptr ctx,
            std::string network,
            int         fd
        )>
        ControlContext;
};

class Listener
{
  public:
    virtual std::tuple<Conn_ptr, error_ptr> Accept() = 0;
    virtual error_ptr                       Close()  = 0;
};

using Listener_ptr = std::shared_ptr<Listener>;

class IP
{
};

class IPMask
{
};

class IPNet
{
};

class IPAddr : public IP
{
    std::string Zone;

    IPMask DefaultMask();
    bool   Equal(IP);
};

class IPConn : public Conn, public PacketConn
{
};

std::tuple<Listener_ptr, error_ptr> FileListener(File_ptr f);

class TCPAddr : public IP
{
  public:
    int         Port;
    std::string Zone;
};

class TCPConn : public Conn
{
};

class TCPListener : public Listener
{
};

class UDPAddr : public IP
{
  public:
    int         Port;
    std::string Zone;
};

class UDPConn : public Conn, public PacketConn
{
};

class UnixAddr : public IP
{
  public:
    std::string Name;
    std::string Net;
};

using UnixAddr_ptr = std::shared_ptr<UnixAddr>;

std::tuple<UnixAddr_ptr, error_ptr> ResolveUnixAddr();

class UnixConn : public Conn
{
  public:
    error_ptr Close();
    error_ptr CloseRead();
    error_ptr CloseWrite();

    std::tuple<File_ptr, error_ptr> File();
};

using UnixConn_ptr = std::shared_ptr<UnixConn>;

std::tuple<UnixConn_ptr, error_ptr> DialUnix(
    std::string  network,
    UnixAddr_ptr laddr,
    UnixAddr_ptr raddr
);

std::tuple<UnixConn_ptr, error_ptr> ListenUnixgram(
    std::string  network,
    UnixAddr_ptr laddr
);

class UnixListener : public Listener
{
};

} // namespace wayround_i2p::akigo::net

#endif
