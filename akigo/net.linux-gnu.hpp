#ifndef WAYROUND_I2P_20240718_170059_653615
#define WAYROUND_I2P_20240718_170059_653615

#include <format>
#include <string>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/errors.hpp>
#include <wayround_i2p/ccutils/akigo/net.hpp>

#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::akigo::net
{

using error_ptr = wayround_i2p::akigo::builtin::error_ptr;
using error     = wayround_i2p::akigo::builtin::error;
using ustring   = wayround_i2p::akigo::builtin::ustring;

using FDCtl = wayround_i2p::ccutils::posix_tools::FDCtl;

// todo: make better CMakeLists.txt and move PosixFDCtlNetConnAdaptor
//       to separate source file

class PosixFDCtlNetConnAdaptor : wayround_i2p::akigo::net::Conn,
                                 wayround_i2p::akigo::net::PacketConn
{
  public:
    static std::shared_ptr<PosixFDCtlNetConnAdaptor> create();

  private:
    std::shared_ptr<FDCtl> fdctl;

    std::weak_ptr<PosixFDCtlNetConnAdaptor> own_ptr;

  protected:
    PosixFDCtlNetConnAdaptor();

  public:
    ~PosixFDCtlNetConnAdaptor();

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
};

class PosixFDCtlNetConnAdaptorAddr : public wayround_i2p::akigo::net::Addr
{
  public:
    ustring Network();
    ustring String();

    static std::tuple<
        std::shared_ptr<PosixFDCtlNetConnAdaptorAddr>,
        int> // todo: use akigo::error ?
        create_for_local(
            std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl
        );

    static std::tuple<
        std::shared_ptr<PosixFDCtlNetConnAdaptorAddr>,
        int> // todo: use akigo::error ?
        create_for_peer(
            std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl
        );

  private:
    static std::tuple<
        std::shared_ptr<PosixFDCtlNetConnAdaptorAddr>,
        int> // todo: use akigo::error ?
        create_for_x(
            std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDCtl> fdctl,
            bool                                                       remote
        );

  protected:
    PosixFDCtlNetConnAdaptorAddr(
        // std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDAddress> addr
    );

  public:
    ~PosixFDCtlNetConnAdaptorAddr();

  private:
    // std::shared_ptr<wayround_i2p::ccutils::posix_tools::FDAddress> addr;
    std::weak_ptr<PosixFDCtlNetConnAdaptorAddr> own_ptr;

    ustring _network;
    ustring _string;
};

} // namespace wayround_i2p::akigo::net

#endif
