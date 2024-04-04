#ifndef WAYROUND_I2P_20240323_153956_24413a
#define WAYROUND_I2P_20240323_153956_24413a

#include <cstdint>
#include <functional>
#include <memory>
#include <tuple>
#include <vector>

#include <fcntl.h>
#include <stddef.h>
#include <sys/socket.h>
#include <time.h>

#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

class FDAddress
{
  private:
    std::vector<std::uint8_t> addr_buff;

    std::waek_ptr<FDAddress> own_ptr;

  public:
    static std::shared_ptr<FDAddress> create();

    // create from existing addr_buff
    static std::shared_ptr<FDAddress> create(
        std::vector<std::uint8_t> addr_buff
    );

    ~FDAddress();

    // set from FDCtl
    int setAddressFromFDCtl(std::shared_ptr<FDCtl> fd);

    // set to custom data
    int setAddrBuff(
        std::vector<std::uint8_t> addr_buff
    );

    std::tuple<sa_family_t, int> getFamily();
    std::tuple<std::string, int> getFamilyString();

    int setAddress(std::string text);

    int setAddress(unsigned char addr[4], in_port_t port);
    int setAddress(std::uint8_t addr[4], in_port_t port);
    int setAddress(std::uint32_t addr, in_port_t port);

    int setAddress(unsigned char addr[16], in_port_t port);
    int setAddress(std::uint8_t addr[16], in_port_t port);

    int setAddress(std::vector<unsigned char> addr);
    int setAddress(std::vector<std::uint8_t> addr);

    int setUnixAddress(std::string text);
    int setUnixAddress(icu::UnicodeString text);

    int setUnixAddress(std::shared_ptr<sockaddr_un> addr);
    int setInetAddress(std::shared_ptr<sockaddr_in> addr);
    int setInet6Address(std::shared_ptr<sockaddr_in6> addr);

    std::tuple<std::shared_ptr<sockaddr_un>, int>  getUnixAddress();
    std::tuple<std::shared_ptr<sockaddr_in>, int>  getInetAddress();
    std::tuple<std::shared_ptr<sockaddr_in6>, int> getInet6Address();

  protected:
    FDAddress();
    FDAddress(std::vector<std::uint8_t> addr_buff);
}

} // namespace wayround_i2p::ccutils::posix_tools

#endif
