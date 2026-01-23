#ifndef WAYROUND_I2P_20251206_235130_79908
#define WAYROUND_I2P_20251206_235130_79908

#include <memory>
#include <tuple>

#include <sys/socket.h>
#include <sys/un.h>

#include <wayround_i2p/ccutils/akigo/net.hpp>
#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::akigo::net
{

/*

// note: UnixAddr should be enough by it self. I doubt UnixAddr should not be final

class LocalUnixAddr;
class LocalUnixAddr_ptr  = std::shared_ptr<LocalUnixAddr>;
using LocalUnixAddr_weak = std::weak_ptr<LocalUnixAddr>;

class LocalUnixAddr
    : final public UnixAddr
{
};
*/

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
    std::tuple<Conn_ptr, error_ptr>
        Accept();

    std::tuple<UnixConn_ptr, error_ptr>
        AcceptUnix();

    std::tuple<Addr_ptr, error_ptr>
        Addr();

    error_ptr
        Close();

    std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr>
        File();

    error_ptr
        SetDeadline(wayround_i2p::akigo::time::Time t);

    void
        SetUnlinkOnClose(bool unlink);
};

class LocalUnixConn;
using LocalUnixConn_ptr  = std::shared_ptr<LocalUnixConn>;
using LocalUnixConn_weak = std::weak_ptr<LocalUnixConn>;

class LocalUnixConn final
    : public wayround_i2p::akigo::net::LocalConn
    , public wayround_i2p::akigo::net::UnixConn
{
  private:
    wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl;
    bool                                          is_open = true;

  public:
    static std::tuple<LocalUnixConn_ptr, error_ptr>
        create_for_FDCtl(
            wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
        );

  protected:
    LocalUnixConn(
        wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
    );

  public:
    ~LocalUnixConn();

  private:
    LocalUnixConn_weak own_ptr;

  public:
    std::tuple<int, UnixAddr_ptr, error_ptr> ReadFromUnix(byte_slice b);

    std::tuple<
        int,          // n
        int,          // oobn
        int,          // flags
        UnixAddr_ptr, // addr
        error_ptr     // error
        >
        ReadMsgUnix(
            byte_slice b,
            byte_slice oob
        );

    std::tuple<
        int,      // n
        int,      // oobn
        error_ptr // err
        >
        WriteMsgUnix(
            byte_slice   b,
            byte_slice   oob,
            UnixAddr_ptr addr
        );

    std::tuple<int, error_ptr> WriteToUnix(byte_slice b, UnixAddr_ptr addr);
};

} // namespace wayround_i2p::akigo::net

#endif
