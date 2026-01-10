#ifndef WAYROUND_I2P_20250205_113427_118553
#define WAYROUND_I2P_20250205_113427_118553

#include <memory>
#include <string>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/context.hpp>
#include <wayround_i2p/ccutils/akigo/io.hpp>
#include <wayround_i2p/ccutils/akigo/os.hpp>
#include <wayround_i2p/ccutils/akigo/time.hpp>

// on GNU+Linux akigo uses posix interface to implement Go's-like net-like
// functionality.
//
// here is Golang documentation quote with all it's
// supported net names used net package
//
// Known values for network parameter are:
//
//         "tcp"
//        "tcp4"   (IPv4-only)
//        "tcp6"   (IPv6-only)
//         "udp"
//        "udp4"   (IPv4-only)
//        "udp6"   (IPv6-only)
//          "ip"
//         "ip4"   (IPv4-only)
//         "ip6"   (IPv6-only)
//
//        "unix"
//    "unixgram"
//  "unixpacket"
//
//  In posix, ip and tcp/udp are separate things,
//    which means there is no tcp4/6 udp4/6 etc in posix
//    and number is reference to ipv4/ipv6 correspondingly.
//
//  So in posix, Golang's "tcp4" means `socket(AF_INET, SOCK_STREAM, 0)`
//    (man 2 socket)

namespace wayround_i2p::akigo::net
{
using error_ptr   = wayround_i2p::akigo::builtin::error_ptr;
using error       = wayround_i2p::akigo::builtin::error;
using ustring     = wayround_i2p::akigo::builtin::ustring;
using size_type   = wayround_i2p::akigo::builtin::size_type;
using byte_slice  = wayround_i2p::akigo::builtin::byte_slice;
using Context_ptr = wayround_i2p::akigo::context::Context_ptr;
using File_ptr    = wayround_i2p::akigo::os::File_ptr;

class Error
    : public error
{
    virtual bool Timeout() = 0;
};

class Addr
{
  public:
    virtual ustring Network() = 0;
    virtual ustring String()  = 0;
};

using Addr_ptr = std::shared_ptr<Addr>;

class Addressed
{
  public:
    // LocalAddr() Addr
    virtual Addr_ptr LocalAddr()  = 0;
    // RemoteAddr() Addr
    virtual Addr_ptr RemoteAddr() = 0;
};

/**
 * WARNING!: in distinction to Go's net.Conn, akigo's [net::Conn]s support
 *           non-blocking states via 3 additional functions
 */
class Conn
    : public Addressed
    , public wayround_i2p::akigo::io::ReadWriteCloser
    , public wayround_i2p::akigo::io::Deadlined
{
};

using Conn_ptr = std::shared_ptr<Conn>;

class PacketConn
    : public Conn
    , public wayround_i2p::akigo::io::ReadWriterFromTo
{
};

using PacketConn_ptr = std::shared_ptr<PacketConn>;

std::tuple<Conn_ptr, error_ptr> Dial(
    ustring network,
    ustring address
);

std::tuple<Conn_ptr, error_ptr> DialTimeout(
    ustring                             network,
    ustring                             address,
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

class Listener;

using Listener_ptr = std::shared_ptr<Listener>;

class Listener
{
  public:
    // Accept() (Conn, error)
    virtual std::tuple<Conn_ptr, error_ptr> Accept() = 0;

    // Close() error
    virtual error_ptr Close() = 0;

    // Addr() Addr
    virtual Addr_ptr Addr() = 0;
};

class IP
{
};

class IPMask
{
};

class IPNet
{
};

class IPAddr
    : public IP
{
    std::string Zone;

    IPMask DefaultMask();
    bool   Equal(IP);
};

class IPConn
    : public Conn
    , public PacketConn
{
};

std::tuple<Listener_ptr, error_ptr> FileListener(File_ptr f);

class TCPAddr
    : public IP
{
  public:
    int     Port;
    ustring Zone;
};

class TCPConn
    : public Conn
{
};

class TCPListener
    : public Listener
{
};

class UDPAddr
    : public IP
{
  public:
    int     Port;
    ustring Zone;
};

class UDPConn
    : public Conn
    , public PacketConn
{
};

// todo: move unix-related structures and other
//       unix-related entities to net_unix.[hc]pp files

class UnixAddr // : public IP
{
  public:
    ustring Name;
    ustring Net;
};

using UnixAddr_ptr = std::shared_ptr<UnixAddr>;

std::tuple<UnixAddr_ptr, error_ptr> ResolveUnixAddr();

class UnixConn
    : public PacketConn
    , public Buffered
    , public PartialCloser
    , public Filed
{
  public:
    // func (c *UnixConn) ReadFromUnix(b []byte) (int, *UnixAddr, error)
    virtual std::tuple<int, UnixAddr_ptr, error_ptr> ReadFromUnix(byte_slice b) = 0;

    // func (c *UnixConn) ReadMsgUnix(b, oob []byte) (n, oobn, flags int, addr *UnixAddr, err error)
    virtual std::tuple<
        int,          // n
        int,          // oobn
        int,          // flags
        UnixAddr_ptr, // addr
        error_ptr     // error
        >
        ReadMsgUnix(
            byte_slice b,
            byte_slice oob
        )
        = 0;

    // func (c *UnixConn) SyscallConn() (syscall.RawConn, error)

    // func (c *UnixConn) WriteMsgUnix(b, oob []byte, addr *UnixAddr) (n, oobn int, err error)
    // func (c *UnixConn) WriteToUnix(b []byte, addr *UnixAddr) (int, error)
};

using UnixConn_ptr = std::shared_ptr<UnixConn>;

std::tuple<UnixConn_ptr, error_ptr> DialUnix(
    ustring      network,
    UnixAddr_ptr laddr,
    UnixAddr_ptr raddr
);

std::tuple<UnixConn_ptr, error_ptr> ListenUnixgram(
    ustring      network,
    UnixAddr_ptr laddr
);

} // namespace wayround_i2p::akigo::net

#include <wayround_i2p/ccutils/akigo/net_tcp.hpp>
#include <wayround_i2p/ccutils/akigo/net_udp.hpp>
#include <wayround_i2p/ccutils/akigo/net_unix.hpp>

#endif
