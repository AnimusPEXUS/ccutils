#ifndef WAYROUND_I2P_20251107_031643_741407
#define WAYROUND_I2P_20251107_031643_741407

#include <memory>

#include <sys/socket.h>
#include <sys/un.h>

namespace wayround_i2p::akigo::net
{

using error_ptr = wayround_i2p::akigo::builtin::error_ptr;
using error     = wayround_i2p::akigo::builtin::error;
using ustring   = wayround_i2p::akigo::builtin::ustring;

class UnixListener;
using UnixListener_ptr  = std::shared_ptr<UnixListener>;
using UnixListener_weak = std::weak_ptr<UnixListener>;


class UnixConn : public wayround_i2p::akigo::net::Conn
{
};

class UnixListener : public wayround_i2p::akigo::net::Listener
{
  public:
    static UnixListener_ptr create(int type_, ustring path);

  protected:
    UnixListener(int type_, ustring path);

  public:
    ~UnixListener();

  private:
    UnixListener_ptr own_ptr;

  public:
    std::tuple<Conn_ptr, error_ptr> Accept();
    error_ptr                       Close();
};


} // namespace wayround_i2p::akigo::net

#endif
