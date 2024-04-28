#ifndef WAYROUND_I2P_20240408_004458_734600
#define WAYROUND_I2P_20240408_004458_734600

#include <wayround_i2p/ccutils/akigo/net.hpp>

#include <wayround_i2p/posix-tools/FDCtl.hpp>

namespace wayround_i2p::akigo::net
{

class PosixFDCtlNetConnAdaptor : wayround_i2p::akigo::net::Conn,
                                 wayround_i2p::akigo::net::PacketConn
{
  private:
    std::shared_ptr<FDCtl> fd;

  protected:
    PosixFDCtlNetConnAdaptor(std::shared_ptr<FDCtl> fd);

  public:
    static std::shared_ptr<PosixFDCtlNetConnAdaptor> create();

    ~PosixFDCtlNetConnAdaptor();

    void                   setFDCtl(std::shared_ptr<FDCtl> fd);
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
