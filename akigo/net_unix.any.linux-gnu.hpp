#ifndef WAYROUND_I2P_20251206_235130_79908
#define WAYROUND_I2P_20251206_235130_79908

#include <memory>
#include <tuple>

#include <sys/socket.h>
#include <sys/un.h>

#include "net_unix.hpp"

namespace wayround_i2p::akigo::net
{

class LocalUnixListener;
using LocalUnixListener_ptr  = std::shared_ptr<LocalUnixListener>;
using LocalUnixListener_weak = std::weak_ptr<LocalUnixListener>;

class LocalUnixListener : public wayround_i2p::akigo::net::UnixListener
{
  public:
    static std::tuple<LocalUnixListener_ptr, error_ptr> create(ustring network, UnixAddr_ptr laddr);

  protected:
    LocalUnixListener(ustring network, UnixAddr_ptr laddr);

  public:
    ~LocalUnixListener();

  private:
    LocalUnixListener_ptr own_ptr;

  public:
    std::tuple<Conn_ptr, error_ptr> Accept();

    std::tuple<UnixConn_ptr, error_ptr> AcceptUnix();

    std::tuple<wayround_i2p::akigo::net::Addr, error_ptr> Addr();

    error_ptr Close();

    std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> File();

    error_ptr SetDeadline(wayround_i2p::akigo::time::Time t);

    void SetUnlinkOnClose(bool unlink);
};

} // namespace wayround_i2p::akigo::net

#endif
