#ifndef WAYROUND_I2P_20251125_162358_230372
#define WAYROUND_I2P_20251125_162358_230372

#include <format>
#include <string>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/errors.hpp>
#include <wayround_i2p/ccutils/akigo/net.hpp>

#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

// todo: make better CMakeLists.txt and move PosixFDCtlNetConnAdapter
//       to separate source file
// todo: ok?

// todo: rename PosixFDCtlNetConnAdapter to PosixFDCtlAkigoNetAdapter
// todo: rename still needed?

namespace wayround_i2p::akigo::net
{

using error_ptr = wayround_i2p::akigo::builtin::error_ptr;
using error     = wayround_i2p::akigo::builtin::error;
using ustring   = wayround_i2p::akigo::builtin::ustring;

using FDCtl      = wayround_i2p::ccutils::posix_tools::FDCtl;
using FDCtl_ptr  = wayround_i2p::ccutils::posix_tools::FDCtl_ptr;
using FDCtl_weak = wayround_i2p::ccutils::posix_tools::FDCtl_weak;

class PosixFDCtlNetConnAdapterAddr;

using PosixFDCtlNetConnAdapterAddr_ptr  = std::shared_ptr<PosixFDCtlNetConnAdapterAddr>;
using PosixFDCtlNetConnAdapterAddr_weak = std::weak_ptr<PosixFDCtlNetConnAdapterAddr>;

class PosixFDCtlNetConnAdapter;

using PosixFDCtlNetConnAdapter_ptr  = std::shared_ptr<PosixFDCtlNetConnAdapter>;
using PosixFDCtlNetConnAdapter_weak = std::weak_ptr<PosixFDCtlNetConnAdapter>;

class PosixFDCtlNetConnAdapter : wayround_i2p::akigo::net::Conn,
                                 wayround_i2p::akigo::net::PacketConn,
                                 wayround_i2p::akigo::net::Listener
{
  public:
    static PosixFDCtlNetConnAdapter_ptr create();

  private:
    std::shared_ptr<FDCtl> fdctl;

    PosixFDCtlNetConnAdapter_weak own_ptr;

  protected:
    PosixFDCtlNetConnAdapter();

  public:
    ~PosixFDCtlNetConnAdapter();

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

    std::tuple<Conn_ptr, error_ptr> Accept();
};

class PosixFDCtlNetConnAdapterAddr : public wayround_i2p::akigo::net::Addr
{
  public:
    ustring Network();
    ustring String();

    static std::tuple<
        std::shared_ptr<PosixFDCtlNetConnAdapterAddr>,
        int> // todo: use akigo::error ?
        create_for_local(
            std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl
        );

    static std::tuple<
        std::shared_ptr<PosixFDCtlNetConnAdapterAddr>,
        int> // todo: use akigo::error ?
        create_for_peer(
            std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl
        );

  private:
    static std::tuple<
        std::shared_ptr<PosixFDCtlNetConnAdapterAddr>,
        int> // todo: use akigo::error ?
        create_for_x(
            std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl,
            bool                                                       remote
        );

  protected:
    PosixFDCtlNetConnAdapterAddr(
        // std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDAddress> addr
    );

  public:
    ~PosixFDCtlNetConnAdapterAddr();

  private:
    // std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDAddress> addr;
    std::weak_ptr<PosixFDCtlNetConnAdapterAddr> own_ptr;

    ustring _network;
    ustring _string;
};

} // namespace wayround_i2p::akigo::net

#endif
