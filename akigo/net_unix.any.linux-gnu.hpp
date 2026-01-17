#ifndef WAYROUND_I2P_20251206_235130_79908
#define WAYROUND_I2P_20251206_235130_79908

#include <memory>
#include <tuple>

#include <sys/socket.h>
#include <sys/un.h>

#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

#include "net_unix.hpp"

namespace wayround_i2p::akigo::net
{

class LocalUnixListener;
using LocalUnixListener_ptr  = std::shared_ptr<LocalUnixListener>;
using LocalUnixListener_weak = std::weak_ptr<LocalUnixListener>;

class LocalUnixListener final
    : public UnixListener
{
  private:
    wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl;

  public:
    static std::tuple<LocalUnixListener_ptr, error_ptr>
        create(ustring network, UnixAddr_ptr laddr);

  protected:
    LocalUnixListener(
        ustring      network,
        UnixAddr_ptr laddr,
        error_ptr   &err
    );

  public:
    ~LocalUnixListener();

  private:
    LocalUnixListener_ptr own_ptr;

  public:
    std::tuple<Conn_ptr, error_ptr> Accept();

    std::tuple<UnixConn_ptr, error_ptr> AcceptUnix();

    std::tuple<Addr_ptr, error_ptr> Addr();

    error_ptr Close();

    std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> File();

    error_ptr SetDeadline(wayround_i2p::akigo::time::Time t);

    void SetUnlinkOnClose(bool unlink);
};

class LocalUnixConn;
using LocalUnixConn_ptr  = std::shared_ptr<LocalUnixConn>;
using LocalUnixConn_weak = std::weak_ptr<LocalUnixConn>;

class LocalUnixConn final
    : public wayround_i2p::akigo::net::UnixConn
{
  private:
    wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl;

  public:
    static std::tuple<LocalUnixConn_ptr, error_ptr>
        create(ustring network, UnixAddr_ptr laddr);

    static std::tuple<LocalUnixConn_ptr, error_ptr> create_for_accepted_FDCtl(
        wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
    );

  protected:
    LocalUnixConn(
        ustring      network,
        UnixAddr_ptr laddr,
        error_ptr   &err
    );

  public:
    ~LocalUnixConn();

  private:
    LocalUnixConn_ptr own_ptr;

  public:
};

} // namespace wayround_i2p::akigo::net

#endif
