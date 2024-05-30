#ifndef WAYROUND_I2P_20240527_064240_391365
#define WAYROUND_I2P_20240527_064240_391365

#include <cstdint>
#include <functional>
#include <memory>
#include <tuple>
#include <vector>

#include <fcntl.h>
#include <stddef.h>
#include <time.h>

#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/un.h>

#include <wayround_i2p/ccutils/unicode/u.hpp>

// #include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

class FDAddress
{
  private:
    std::vector<std::uint8_t> addr_buff;

    std::weak_ptr<FDAddress> own_ptr;

  public:
    static std::shared_ptr<FDAddress> create();

    // create from existing addr_buff
    static std::shared_ptr<FDAddress> create(
        std::vector<std::uint8_t> addr_buff
    );

    // create from memory
    static std::shared_ptr<FDAddress> create(
        struct sockaddr *addr,
        socklen_t        length
    );

    ~FDAddress();

    /*
    // set from FDCtl
    err_errNoS setAddressFromFDCtl(std::shared_ptr<FDCtl> fd);
    */

    // set to custom data
    int setAddrBuff(std::vector<std::uint8_t> addr_buff);

    // get raw sockaddr data
    std::vector<std::uint8_t> getAddrBuff();

    std::tuple<sa_family_t, int> getFamily();
    std::tuple<std::string, int> getFamilyString();

    int setInetAddress(wayround_i2p::ccutils::unicode::UString text, in_port_t port);

    int setInetAddress(std::vector<std::uint8_t> addr, in_port_t port);
    int setInetAddress(std::array<std::uint8_t, 4> addr, in_port_t port);
    int setInetAddress(std::uint32_t addr, in_port_t port);
    int setInet6Address(std::array<std::uint8_t, 16> addr, in_port_t port);

    int setUnixAddress(wayround_i2p::ccutils::unicode::UString text);

    int setUnixAddress(std::shared_ptr<sockaddr_un> addr);
    int setInetAddress(std::shared_ptr<sockaddr_in> addr);
    int setInet6Address(std::shared_ptr<sockaddr_in6> addr);

    std::tuple<wayround_i2p::ccutils::unicode::UString, int> getUnixAddress();
    std::tuple<std::array<std::uint8_t, 4>, in_port_t, int>  getInetAddress();
    std::tuple<std::array<std::uint8_t, 16>, in_port_t, int> getInet6Address();

    std::tuple<std::shared_ptr<sockaddr_un>, int>  get_sockaddr_un();
    std::tuple<std::shared_ptr<sockaddr_in>, int>  get_sockaddr_in();
    std::tuple<std::shared_ptr<sockaddr_in6>, int> get_sockaddr_in6();

  protected:
    FDAddress();
    FDAddress(std::vector<std::uint8_t> addr_buff);
};

} // namespace wayround_i2p::ccutils::posix_tools

#endif
