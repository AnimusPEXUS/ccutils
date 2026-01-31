#ifndef WAYROUND_I2P_20260131_182305_0000230965
#define WAYROUND_I2P_20260131_182305_0000230965

#include <wayround_i2p/ccutils/akigo/net.hpp>
#include <wayround_i2p/ccutils/akigo/os.hpp>
#include <wayround_i2p/ccutils/akigo/time.hpp>

#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::akigo::net
{

class LocalConn;
using LocalConn_ptr  = std::shared_ptr<LocalConn>;
using LocalConn_weak = std::weak_ptr<LocalConn>;

class LocalConn
    : public virtual PacketConn
    , public virtual PartialCloser
    , public virtual Buffered
    , public virtual wayround_i2p::akigo::io::Filed
{
  private:
    wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl;
    bool                                          is_open = true;

  protected:
    LocalConn(
        wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
    );

  public:
    ~LocalConn();

  public:
    void set_fdctl(
        wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
    );
    wayround_i2p::ccutils::posix_tools::FDCtl_ptr get_fdctl();

    std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> File();
    std::tuple<go_int, error_ptr>                            Read(byte_slice p);
    std::tuple<go_int, error_ptr>                            Write(byte_slice p);
    error_ptr                                                Close();
    error_ptr                                                CloseRead();
    error_ptr                                                CloseWrite();
    error_ptr                                                SetReadBuffer(int bytes);
    error_ptr                                                SetWriteBuffer(int bytes);
    error_ptr                                                SetDeadline(wayround_i2p::akigo::time::Time t);
    error_ptr                                                SetReadDeadline(wayround_i2p::akigo::time::Time t);
    error_ptr                                                SetWriteDeadline(wayround_i2p::akigo::time::Time t);
    std::tuple<int, Addr_ptr, error_ptr>                     ReadFrom(byte_slice p);
    std::tuple<int, error_ptr>                               WriteTo(byte_slice p, Addr_ptr addr);
    Addr_ptr                                                 LocalAddr();
    Addr_ptr                                                 RemoteAddr();
};

} // namespace wayround_i2p::akigo::net

#endif
