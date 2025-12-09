#ifndef WAYROUND_I2P_20251206_235130_80417
#define WAYROUND_I2P_20251206_235130_80417

#include <memory>
#include <tuple>

#include "net.hpp"

namespace wayround_i2p::akigo::net
{

class UnixListener;
using UnixListener_ptr  = std::shared_ptr<UnixListener>;
using UnixListener_weak = std::weak_ptr<UnixListener>;

class UnixListener : public wayround_i2p::akigo::net::Listener
{
  public:
    // func (l *UnixListener) Accept() (Conn, error)
    virtual std::tuple<Conn_ptr, error_ptr> Accept() = 0;

    // func (l *UnixListener) AcceptUnix() (*UnixConn, error)
    virtual std::tuple<UnixConn_ptr, error_ptr> AcceptUnix() = 0;

    // func (l *UnixListener) Addr() Addr
    virtual std::tuple<wayround_i2p::akigo::net::Addr, error_ptr> Addr() = 0;

    // func (l *UnixListener) Close() error
    virtual error_ptr Close() = 0;

    // func (l *UnixListener) File() (f *os.File, err error)
    virtual std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> File() = 0;

    // func (l *UnixListener) SetDeadline(t time.Time) error
    virtual error_ptr SetDeadline(wayround_i2p::akigo::time::Time t) = 0;

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
