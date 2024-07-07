#ifndef WAYROUND_I2P_20240627_141739_95392
#define WAYROUND_I2P_20240627_141739_95392

#include <cstring>
#include <format>

#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::ip
{

using error_ptr = wayround_i2p::ccutils::errors::error_ptr;
using UString   = wayround_i2p::ccutils::unicode::UString;

class IP
{
  public:
    static std::shared_ptr<IP>
        create();

    static std::tuple<std::shared_ptr<IP>, error_ptr>
        createFromArray(std::array<std::uint8_t, 4> arr);

    static std::tuple<std::shared_ptr<IP>, error_ptr>
        createFromArray(std::array<std::uint8_t, 16> arr);

    static std::tuple<std::shared_ptr<IP>, error_ptr>
        createFromArray(std::array<std::uint16_t, 8> arr);

    static std::tuple<std::shared_ptr<IP>, error_ptr>
        createFromVector(std::vector<std::uint8_t> vec);

    static std::tuple<std::shared_ptr<IP>, error_ptr>
        createFromVector(std::vector<std::uint16_t> vec);

    static std::tuple<std::shared_ptr<IP>, error_ptr>
        createFromString(UString text);

    error_ptr
        setFromArray(std::array<std::uint8_t, 4> arr);
    error_ptr
        setFromArray(std::array<std::uint8_t, 16> arr);
    error_ptr
        setFromArray(std::array<std::uint16_t, 8> arr);
    error_ptr
        setFromVector(std::vector<std::uint8_t> vec);
    error_ptr
        setFromVector(std::vector<std::uint16_t> vec);
    error_ptr
        setFromString(UString text);

    bool
        isSet();

    // get bytes size
    std::tuple<int, error_ptr>
        getSize();
    std::tuple<int, error_ptr>
        getVer();

    std::tuple<UString, error_ptr>
        toString();
    std::tuple<UString, error_ptr>
        to4String();
    std::tuple<UString, error_ptr>
        to6String();

  protected:
    IP();

  public:
    ~IP();

  private:
    std::vector<std::uint8_t> buff;
};

class Port
{
  public:
    static std::shared_ptr<Port>
        create();

    static std::tuple<std::shared_ptr<Port>, error_ptr>
        createFromUInt16(std::uint16_t port);

    static std::tuple<std::shared_ptr<Port>, error_ptr>
        createFromString(UString text);

    error_ptr
        setFromUInt16(std::uint16_t port);
    error_ptr
        setFromString(UString text);

    std::tuple<UString, error_ptr> toString();

  protected:
    Port();

  public:
    ~Port();

  private:
    std::uint16_t port;
};

class IPAndPort
{
  public:
    static std::shared_ptr<IPAndPort>
        create();

    static std::tuple<std::shared_ptr<IPAndPort>, error_ptr>
        createFromString(UString text);

    error_ptr
        setFromString(UString text);

    error_ptr
        setIP(std::shared_ptr<IP> ip);
    error_ptr
        setPort(std::shared_ptr<Port> port);

    std::shared_ptr<IP>
        getIP();
    std::shared_ptr<Port>
        getPort();

    void
        delIP();
    void
        delPort();

    std::tuple<UString, error_ptr>
        toString();

  protected:
    IPAndPort();

  public:
    ~IPAndPort();

  private:
    std::shared_ptr<IP>   ip;
    std::shared_ptr<Port> port;
};

} // namespace wayround_i2p::ccutils::ip

#endif
