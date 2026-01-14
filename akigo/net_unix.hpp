#ifndef WAYROUND_I2P_20251206_235130_80417
#define WAYROUND_I2P_20251206_235130_80417

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/io.hpp>
#include <wayround_i2p/ccutils/akigo/net.hpp>

namespace wayround_i2p::akigo::net
{

class UnixListener;
using UnixListener_ptr  = std::shared_ptr<UnixListener>;
using UnixListener_weak = std::weak_ptr<UnixListener>;

class UnixListener
    : public Listener
    , public wayround_i2p::akigo::io::Filed
{
  public:
    // func (l *UnixListener) AcceptUnix() (*UnixConn, error)
    virtual std::tuple<UnixConn_ptr, error_ptr> AcceptUnix() = 0;

    // func (l *UnixListener) SetUnlinkOnClose(unlink bool)
    virtual void SetUnlinkOnClose(bool unlink) = 0;

    // todo: todo
    // func (l *UnixListener) SyscallConn() (syscall.RawConn, error)
    // std::tuple<wayround_i2p::akigo::syscall::RawConn_ptr, error_ptr> SyscallConn();
};

std::tuple<UnixListener_ptr, error_ptr> ListenUnix(ustring network, UnixAddr_ptr laddr);

} // namespace wayround_i2p::akigo::net

#include "net_unix._local_headers.hpp"

#endif
