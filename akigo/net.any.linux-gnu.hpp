#ifndef WAYROUND_I2P_20260123_115658_470371
#define WAYROUND_I2P_20260123_115658_470371

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
    : public virtual wayround_i2p::akigo::net::Conn
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
    std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> File();
    std::tuple<go_int, error_ptr>                            Read(byte_slice p);
    std::tuple<go_int, error_ptr>                            Write(byte_slice p);
    error_ptr                                                Close();
    error_ptr                                                CloseRead();
    error_ptr                                                CloseWrite();
    error_ptr                                                SetDeadline(wayround_i2p::akigo::time::Time t);
    error_ptr                                                SetReadDeadline(wayround_i2p::akigo::time::Time t);
    error_ptr                                                SetWriteDeadline(wayround_i2p::akigo::time::Time t);
    std::tuple<go_int64, error_ptr>                          WriteTo(wayround_i2p::akigo::io::Writer_ptr w);
    std::tuple<go_int64, error_ptr>                          ReadFrom(wayround_i2p::akigo::io::Reader_ptr r);
    error_ptr                                                SetReadBuffer(int bytes);
    error_ptr                                                SetWriteBuffer(int bytes);
};

} // namespace wayround_i2p::akigo::net

#endif
