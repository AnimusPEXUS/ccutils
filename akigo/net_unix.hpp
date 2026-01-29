#ifndef WAYROUND_I2P_20251206_235130_80417
#define WAYROUND_I2P_20251206_235130_80417

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/io.hpp>
#include <wayround_i2p/ccutils/akigo/net.hpp>

namespace wayround_i2p::akigo::net
{

class UnixAddr final
    : public virtual Addr
{
  public:
    ustring Name;
    ustring Net;

    ustring Network();
    ustring String();
};

using UnixAddr_ptr = std::shared_ptr<UnixAddr>;

std::tuple<UnixAddr_ptr, error_ptr> ResolveUnixAddr();

class UnixConn;
using UnixConn_ptr  = std::shared_ptr<UnixConn>;
using UnixConn_weak = std::weak_ptr<UnixConn>;

class UnixConn
    : public virtual PacketConn
    , public virtual PartialCloser
    , public virtual Buffered
    , public virtual wayround_i2p::akigo::io::Filed
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

    // func (c *UnixConn) WriteMsgUnix(b, oob []byte, addr *UnixAddr) (n, oobn int, err error)
    virtual std::tuple<
        int,      // n
        int,      // oobn
        error_ptr // err
        >
        WriteMsgUnix(
            byte_slice   b,
            byte_slice   oob,
            UnixAddr_ptr addr
        )
        = 0;

    // func (c *UnixConn) WriteToUnix(b []byte, addr *UnixAddr) (int, error)
    virtual std::tuple<int, error_ptr> WriteToUnix(byte_slice b, UnixAddr_ptr addr) = 0;

    // func (c *UnixConn) SyscallConn() (syscall.RawConn, error)
};

class UnixListener;
using UnixListener_ptr  = std::shared_ptr<UnixListener>;
using UnixListener_weak = std::weak_ptr<UnixListener>;

class UnixListener
    : public virtual Listener
    , public virtual wayround_i2p::akigo::io::Filed
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

std::tuple<UnixConn_ptr, error_ptr> DialUnix(
    ustring      network,
    UnixAddr_ptr laddr,
    UnixAddr_ptr raddr
);

std::tuple<UnixListener_ptr, error_ptr> ListenUnix(
    ustring      network,
    UnixAddr_ptr laddr
);

std::tuple<UnixConn_ptr, error_ptr> ListenUnixgram(
    ustring      network,
    UnixAddr_ptr laddr
);

} // namespace wayround_i2p::akigo::net

#include "net_unix._local_headers.hpp"

#endif
