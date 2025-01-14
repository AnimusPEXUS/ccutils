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

enum class IPver : unsigned char
{
    v4 = 4,
    v6 = 6
};

namespace regexp = wayround_i2p::ccutils::regexp;

inline bool check_val_fits_1_byte(long long val);
inline bool check_val_fits_2_bytes(long long val);

// todo: those regexps were made before ccutisl::regexp got repetitions fix
//       so they may be incorrect or unoptimised

regexp::Pattern_shared IPv4_STR_PATTERN();
regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN();
regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4();
regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN();
regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4();
regexp::Pattern_shared IPv6_STR_PATTERN();
regexp::Pattern_shared IP_STR_PATTERN();

regexp::Pattern_shared PORT_STR_PATTERN();
regexp::Pattern_shared CIDR_STR_PATTERN();
regexp::Pattern_shared PORT_OR_CIDR_STR_PATTERN();

regexp::Pattern_shared IP_ONLY_PATTERN();
regexp::Pattern_shared PORT_ONLY_PATTERN();
regexp::Pattern_shared CIDR_ONLY_PATTERN();
regexp::Pattern_shared PORT_OR_CIDR_ONLY_PATTERN();

regexp::Pattern_shared IP_AND_MUST_PORT_OR_CIDR_PATTERN();
regexp::Pattern_shared IP_AND_MUST_PORT_PATTERN();
regexp::Pattern_shared IP_AND_MUST_CIDR_PATTERN();

regexp::Pattern_shared IP_AND_OPT_PORT_OR_CIDR_PATTERN();
regexp::Pattern_shared IP_AND_OPT_PORT_PATTERN();
regexp::Pattern_shared IP_AND_OPT_CIDR_PATTERN();

regexp::Pattern_shared OPT_IP_AND_MUST_PORT_OR_CIDR_PATTERN();
regexp::Pattern_shared OPT_IP_AND_MUST_PORT_PATTERN();
regexp::Pattern_shared OPT_IP_AND_MUST_CIDR_PATTERN();

error_ptr
    getValueFrom_PORT_STR_PATTERN_Result(
        const regexp::Result_shared res,
        bool                       &matched,
        std::uint16_t              &value
    );

error_ptr
    getValueFrom_CIDR_STR_PATTERN_Result(
        const regexp::Result_shared res,
        bool                       &matched,
        std::uint16_t              &value
    );

error_ptr
    getValueFrom_port_or_cidr_STR_PATTERN_Result(
        bool                        port_1_cidr_0,
        const regexp::Result_shared res,
        bool                       &matched,
        std::uint16_t              &value
    );

// 192.168.0.1 -> res[0]=1, res[1]=0, res[2]=168, res[3]=192
error_ptr getValuesFrom_IPv4_STR_PATTERN_Result(
    const regexp::Result_shared  res,
    bool                        &matched,
    std::array<std::uint8_t, 4> &ipv4
);

// set ret_localendian param to true if you want this function to
//     automatically convert ints in ipv6 to local-endian
error_ptr getValuesFrom_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    bool                         &matched,
    std::array<std::uint16_t, 8> &ipv6,
    std::array<std::uint8_t, 4>  &ipv4,
    bool                         &ipv6_short,
    bool                         &ipv6_v4_comb,
    bool                          ret_localendian
);

// lowest index of ip arrays contains lowest ip parts.
// if function detects ipv6 is comb with ipv4 - ipv4 value additionally goes to ipv4.
// set ret_localendian param to true if you want this function to
//     automatically convert ints in ipv6 to local-endian.
// ipv4 result is not affected by ret_localendian.
// OPT_IP_AND_MUST_PORT_OR_CIDR_PATTERN() is recommended function as a source
// for this res param.
error_ptr getAllPossibleValuesFromResult(
    const regexp::Result_shared   res,
    IPver                        &ipver,
    bool                         &ipv6_matched,
    std::array<std::uint16_t, 8> &ipv6,
    bool                         &ipv4_matched,
    std::array<std::uint8_t, 4>  &ipv4,
    bool                         &ipv6_short,
    bool                         &ipv6_v4_comb,
    bool                         &port_matched,
    std::uint16_t                &port,
    bool                         &cidr_matched,
    std::uint16_t                &cidr,
    bool                          ret_localendian
);

#pragma pack(push, 1)
union IPv6_array
{
    std::array<std::uint8_t, 16> b8;
    std::array<std::uint16_t, 8> b16;
    std::array<std::uint32_t, 4> b32;
};
#pragma pack(pop)

class IP;
using IP_shared = std::shared_ptr<IP>;
using IP_weak   = std::weak_ptr<IP>;

/*
  note: In IPv6 setter functions, if 3rd ip value equals to ffff,
        ipv6_v4_comb is automatically set to true.
  note: In `set*` functions, if you see `bool big = false` parameter,
        you can set it to true if you passing integers in bigendian order,
        else, if you passing integers in local endianness keep `big == false`.
        In case of `get*` functions, if you pass big as true - you'll get
        integers in bigendian, else in local endian.
*/
class IP
{
  protected:
    IP();

  public:
    ~IP();

    void clear();

    static IP_shared create();

    error_ptr setAllFromString(const UString &text);

    void setAllFromIP(const IP_shared obj);
    void setIPFromIP(const IP_shared obj);
    void setPortFromIP(const IP_shared obj);
    void setCIDRFromIP(const IP_shared obj);

    void setIPFromArray(const std::array<std::uint8_t, 4> &arr);
    void setIPFromArray(const std::array<std::uint8_t, 16> &arr);
    void setIPFromArray(const std::array<std::uint16_t, 8> &arr, bool big = false);
    void setIPFromArray(const std::array<std::uint32_t, 4> &arr, bool big = false);

    error_ptr setIPFromVector(const std::vector<std::uint8_t> &vec);
    error_ptr setIPFromVector(const std::vector<std::uint16_t> &vec, bool big = false);
    error_ptr setIPFromVector(const std::vector<std::uint32_t> &vec, bool big = false);

    error_ptr setIPFromString(const UString &text);

    // automativaly deletes CIDR
    void setPort(std::uint16_t val);

    // automativaly deletes Port
    void setCIDR(std::uint16_t val);

    IPver getIPver() const;

    bool hasIP() const;
    bool hasIPv4() const;
    bool hasIPv6() const;
    bool hasPort() const;
    bool hasCIDR() const;

    bool isIPv6IPv4combine() const;

    // returns true if ipv6 is stored insude and 3rd uint16 equals ffff.
    bool hasIPv6IPv4CombineMagic() const;

    void setIPv6IPv4Combine(bool val, bool set_ipv4_combine_magic = true);

    // note: this doesn't automatically sets IPv6IPv4combine to true.
    //       use setIPv6IPv4combine() function
    void setIPv6IPv4Part(const std::array<std::uint8_t, 4> &arr);
    void setIPv6IPv4Part(const IP &obj);

    IP_shared getIPv6IPv4Part() const;

    std::array<std::uint8_t, 4>  &getIPAsArray8(std::array<std::uint8_t, 4> &arr) const;
    std::array<std::uint8_t, 16> &getIPAsArray8(std::array<std::uint8_t, 16> &arr) const;
    std::array<std::uint16_t, 8> &getIPAsArray16(std::array<std::uint16_t, 8> &arr, bool big = false) const;
    std::array<std::uint32_t, 4> &getIPAsArray32(std::array<std::uint32_t, 4> &arr, bool big = false) const;

    std::vector<std::uint8_t>  &getIPAsVector8(std::vector<std::uint8_t> &vec) const;
    std::vector<std::uint16_t> &getIPAsVector16(std::vector<std::uint16_t> &vec, bool big = false) const;
    std::vector<std::uint32_t> &getIPAsVector32(std::vector<std::uint32_t> &vec, bool big = false) const;

    std::tuple<std::uint16_t, error_ptr> getPort() const;
    std::tuple<std::uint16_t, error_ptr> getCIDR() const;

    UString getAllAsString() const;
    UString getAllAsLongString() const;
    UString getAllAsShortString() const;

    UString getIPAsString() const;
    UString getIPAsStringLong() const;
    UString getIPAsStringShort() const;

    UString getPortString() const;
    UString getCIDRString() const;

    // todo: make customizable versions of getIPAsString() and getAllAsString()
    //       functions (leading zeroes, skipped null-elements)

  private:
    IP_weak own_ptr;

    IPver ipver;

#pragma pack(push, 1)
    union
    {
        std::array<std::uint8_t, 4> ipv4;
        IPv6_array                  ipv6;
    } buff;
#pragma pack(pop)

    bool ipv6_v4_comb;

    bool has_ip;
    bool has_port;
    bool has_cidr;

    std::uint16_t port;
    std::uint16_t cidr;
};

} // namespace wayround_i2p::ccutils::ip

#endif
