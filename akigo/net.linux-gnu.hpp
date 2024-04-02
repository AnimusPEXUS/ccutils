#ifndef WAYROUND_I2P_20240325_015305_233621
#define WAYROUND_I2P_20240325_015305_233621

#include <wayround_i2p/ccutils/akigo/net.hpp>

#include <wayround_i2p/posix-tools/FDCtl.hpp>

namespace wayround_i2p::akigo::net
{

// on GNU+Linux akigo uses posix interface to implement Go's-like net-like
// functionality.
//
// here is Golang documentation quote with all it's
// supported net names used net package
//
// Known networks are       "tcp", "tcp4"   (IPv4-only),
//   "tcp6"    (IPv6-only), "udp", "udp4"   (IPv4-only),
//   "udp6"  (IPv6 - only),  "ip",  "ip4" (IPv4 - only),
//    "ip6"    (IPv6-only),
//
//   "unix", "unixgram" and "unixpacket".
//
//  In posix, ip and tcp/udp are separate things,
//    which means there is no tcp4/6 udp4/6 etc in posix
//    and number is reference to ipv4/ipv6 correspondingly.
//
//  So in posix, Golang's "tcp4" means `socket(AF_INET, SOCK_STREAM, 0)` 
//    (man 2 socket)

class StdSocketConn : wayround_i2p::akigo::net::Conn,
                      wayround_i2p::akigo::net::PacketConn
{
  private:
    std::shared_ptr<FDCtl> fd;

  protected:
    StdSocketConn(std::shared_ptr<FDCtl> fd);

  public:
    static std::shared_ptr<StdSocketConn> create();
    static std::shared_ptr<StdSocketConn> create(
        int  fd,
        bool close_on_destroy = false
    );
    static std::shared_ptr<StdSocketConn> create(std::shared_ptr<FDCtl> fd);

    ~StdSocketConn();

    std::shared_ptr<FDCtl> getFDCtl();

    error_ptr Close();

    Addr_ptr LocalAddr();
    Addr_ptr RemoteAddr();

    bool SupportsNonBlocking();
    bool GetNonBlocking();
    void SetNonBlocking(bool value);

    error_ptr SetDeadline(wayround_i2p::akigo::time::Time t);
    error_ptr SetReadDeadline(wayround_i2p::akigo::time::Time t);
    error_ptr SetWriteDeadline(wayround_i2p::akigo::time::Time t);

    std::tuple<size_type, error_ptr> Read(byte_vector b);
    std::tuple<size_type, error_ptr> Write(byte_vector b);

    std::tuple<size_type, Addr_ptr, error_ptr> ReadFrom(byte_vector b);
    std::tuple<size_type, error_ptr>           WriteTo(byte_vector b, Addr_ptr addr);
};

} // namespace wayround_i2p::akigo::net

#endif
