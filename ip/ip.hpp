#ifndef WAYROUND_I2P_20241007_110017_746804
#define WAYROUND_I2P_20241007_110017_746804

#include <bit>
#include <cstring>
#include <format>

#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/regexp/regexp.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::ip
{

using error_ptr = wayround_i2p::ccutils::errors::error_ptr;
using UString   = wayround_i2p::ccutils::unicode::UString;

namespace regexp = wayround_i2p::ccutils::regexp;

inline bool check_val_fits_1_byte(long long val);
inline bool check_val_fits_2_bytes(long long val);

// todo: those regexps were made before ccutisl::regexp got repetitions fix
//       so they may be incorrect or unoptimised

regexp::Pattern_shared PORT_STR_PATTERN();
regexp::Pattern_shared CIDR_STR_PATTERN();
regexp::Pattern_shared IPv4_STR_PATTERN();
regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN();
regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4();
regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN();
regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4();
regexp::Pattern_shared IPv6_STR_PATTERN();
regexp::Pattern_shared IP_STR_PATTERN();

enum class IP_AND_CIDR_OR_PORT_STR_PATTERN_mode : unsigned char
{
    ip_and_must_cidr_or_port,
    ip_and_must_cidr,
    ip_and_must_port,
    ip_and_opt_cidr_or_port,
    ip_and_opt_cidr,
    ip_and_opt_port,
    ip_only
};

regexp::Pattern_shared IP_AND_CIDR_OR_PORT_STR_PATTERN(
    IP_AND_CIDR_OR_PORT_STR_PATTERN_mode mode
);

error_ptr
    getValueFrom_PORT_STR_PATTERN_Result(
        bool                        port_1_cidr_0,
        const regexp::Result_shared res,
        bool                       &has_value,
        std::uint16_t              &value
    );

error_ptr
    getValueFrom_CIDR_STR_PATTERN_Result(
        bool                        port_1_cidr_0,
        const regexp::Result_shared res,
        bool                       &has_value,
        std::uint16_t              &value
    );

error_ptr
    getValueFrom_port_or_cidr_STR_PATTERN_Result(
        bool                        port_1_cidr_0,
        const regexp::Result_shared res,
        bool                       &has_value,
        std::uint16_t              &value
    );

// 192.168.0.1 -> res[0]=1, res[1]=0, res[2]=168, res[3]=192
error_ptr getValuesFrom_IPv4_STR_PATTERN_Result(
    const regexp::Result_shared  res,
    std::array<std::uint8_t, 4> &ipv4
);

// 0123:4567:89ab:cdef:0123:4567:89ab:cdef ->
//	res[0]=cdef, res[1]=89ab, res[2]=4567 etc.
//   note: resulting ints are localized to local endianness using std::bytesswap
//         so 0123 on littleendian systems becomes 2301
error_ptr getValuesFrom_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    std::array<std::uint16_t, 8> &ipv6,
    bool                         &ipv6_short,
    bool                         &ipv6_v4_comb
);

// integers in ipv6 array converted to local endianness.
// lowest index of ip arrays contains lowest ip parts.
error_ptr getValuesFrom_IP_AND_CIDR_OR_PORT_STR_PATTERN_ip_and_opt_cidr_or_port_Result(
    const regexp::Result_shared   res,
    bool                         &is_ipv4,
    bool                         &is_ipv6,
    std::array<std::uint8_t, 4>  &ipv4,
    std::array<std::uint16_t, 8> &ipv6,
    bool                         &ipv6_short,
    bool                         &ipv6_v4_comb,
    bool                         &has_cidr,
    std::uint16_t                &cidr,
    bool                         &has_port,
    std::uint16_t                &port
);

class IPv4;

using IPv4_shared = std::shared_ptr<IPv4>;
using IPv4_weak   = std::weak_ptr<IPv4>;

class IPv4
{
  public:
    static IPv4_shared create();

    static IPv4_shared                        createFromArray(const std::array<std::uint8_t, 4> &arr);
    static std::tuple<IPv4_shared, error_ptr> createFromVector(const std::vector<std::uint8_t> &vec);
    static std::tuple<IPv4_shared, error_ptr> createFromString(const UString &val);
    static std::tuple<IPv4_shared, error_ptr> createFromParsedString(const regexp::Result_shared &res);

    void      setFromArray(const std::array<std::uint8_t, 4> &arr);
    error_ptr setFromVector(const std::vector<std::uint8_t> &vec);
    error_ptr setFromString(const UString &val);
    error_ptr setFromParsedString(const regexp::Result_shared &res);

    UString                     toString() const;
    std::array<std::uint8_t, 4> toArray() const;
    std::vector<std::uint8_t>   toVector() const;

  protected:
    IPv4();

  public:
    ~IPv4();

  private:
    std::array<std::uint8_t, 4> buff;
    IPv4_weak                   own_ptr;
};

class IPv6;

using IPv6_shared = std::shared_ptr<IPv6>;
using IPv6_weak   = std::weak_ptr<IPv6>;

union IPv6_array
{
    std::array<std::uint8_t, 16> b8;
    std::array<std::uint16_t, 8> b16;
    std::array<std::uint32_t, 4> b32;
};

class IPv6
{
  public:
    static IPv6_shared create();

    static IPv6_shared                        createFromArray(const std::array<std::uint8_t, 16> &arr);
    static IPv6_shared                        createFromArray(const std::array<std::uint16_t, 8> &arr);
    static IPv6_shared                        createFromArray(const std::array<std::uint32_t, 4> &arr);
    // static IPv6_shared                        createFromArray(const IPv6_array &arr);
    static std::tuple<IPv6_shared, error_ptr> createFromVector(const std::vector<std::uint8_t> &vec);
    static std::tuple<IPv6_shared, error_ptr> createFromVector(const std::vector<std::uint16_t> &vec);
    static std::tuple<IPv6_shared, error_ptr> createFromVector(const std::vector<std::uint32_t> &vec);
    static std::tuple<IPv6_shared, error_ptr> createFromString(const UString &text);

    void      setFromArray(const std::array<std::uint8_t, 16> &arr);
    void      setFromArray(const std::array<std::uint16_t, 8> &arr);
    void      setFromArray(const std::array<std::uint32_t, 4> &arr);
    // void      setFromArray(const IPv6_array &arr);
    error_ptr setFromVector(const std::vector<std::uint8_t> &vec);
    error_ptr setFromVector(const std::vector<std::uint16_t> &vec);
    error_ptr setFromVector(const std::vector<std::uint32_t> &vec);
    error_ptr setFromString(const UString &text);

    std::array<std::uint8_t, 16> &toArray8(std::array<std::uint8_t, 16> &arr) const;
    std::array<std::uint16_t, 8> &toArray16(std::array<std::uint16_t, 8> &arr) const;
    std::array<std::uint32_t, 4> &toArray32(std::array<std::uint32_t, 4> &arr) const;
    std::vector<std::uint8_t>    &toVector8(std::vector<std::uint8_t> &vec) const;
    std::vector<std::uint16_t>   &toVector16(std::vector<std::uint16_t> &vec) const;
    std::vector<std::uint32_t>   &toVector32(std::vector<std::uint32_t> &vec) const;
    UString                       toString() const;
    UString                       toStringLong() const;
    UString                       toStringShort() const;

    void        setIPv4Comb(bool val = true);
    void        setIPv4Comb(const IPv4_shared &comb_part);
    bool        isIPv4Comb() const;
    IPv4_shared getIPv4Comb() const;

  protected:
    IPv6();

  public:
    ~IPv6();

  private:
    IPv6_array buff;
    IPv6_weak  own_ptr;
    bool       ipv4_comb = false;
};

// todo: is this unused?
enum class IPCombine_mode : unsigned char
{
    v4,
    v6
};

template <typename T>
concept IPvType = (std::same_as<T, IPv4> || std::same_as<T, IPv6>);

template <IPvType T>
struct IPCombine_parts
{
    IPCombine_mode     mode;
    std::shared_ptr<T> addr;
    bool               has_port;
    std::uint16_t      port;
    std::shared_ptr<T> mask;
    bool               has_cidr;
    std::uint16_t      cidr;
};

template <IPvType T>
class IPCombine;

template <IPvType T>
using IPCombine_shared = std::shared_ptr<IPCombine<T>>;

template <IPvType T>
using IPCombine_weak = std::weak_ptr<IPCombine<T>>;

template <IPvType T>
class IPCombine
{
  public:
    static IPCombine_shared<T> createFromParts(
        IPCombine_parts<T> &parts
    );

    void setIP(T val);
    void setPort(std::uint16_t val);
    void setCIDR(std::uint16_t);

    void delIP();
    void delPort();
    void delCIDR();

    bool hasIP();
    bool hasPort();
    bool hasCIDR();

    T             getIP() const;
    std::uint16_t getPort() const;
    std::uint16_t getCIDR() const;

  protected:
    IPCombine();

  public:
    ~IPCombine();

  private:
    IPCombine_weak<T> own_ptr;

    std::shared_ptr<T> ip;

    bool has_port;
    bool has_cidr;

    std::uint16_t port;
    // todo: type? - make variable type depending on template?
    std::uint16_t cidr;
};

} // namespace wayround_i2p::ccutils::ip

#endif
