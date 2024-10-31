#ifndef WAYROUND_I2P_20241007_110017_746804
#define WAYROUND_I2P_20241007_110017_746804

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

constexpr regexp::Pattern_shared PORT_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
            {regexp::Pattern::newExactChar(':'),
             regexp::Pattern::newCharIsDigit()
                 ->setName("number")
                 ->setMinCount(1)
                 ->unsetMaxCount()
            }
        );
    return ret;
}

std::tuple<std::uint16_t, error_ptr>
    getNumberFrom_PORT_STR_PATTERN_Result(
        const regexp::Result_shared res
    );

constexpr regexp::Pattern_shared CIDR_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
            {regexp::Pattern::newExactChar('/'),
             regexp::Pattern::newCharIsDigit()
                 ->setName("number")
                 ->setMinCount(1)
                 ->unsetMaxCount()
            }
        );
    return ret;
}

std::tuple<std::uint16_t, error_ptr>
    getNumberFrom_CIDR_STR_PATTERN_Result(
        const regexp::Result_shared res
    );

constexpr regexp::Pattern_shared IPv4_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("1"),
               regexp::Pattern::newExactChar('.')
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("2"),
               regexp::Pattern::newExactChar('.')
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("3"),
               regexp::Pattern::newExactChar('.')
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("4")
              }
        )
              ->setName("IPv4_STR_PATTERN");
    return ret;
}

error_ptr getIPBytesFrom_IPv4_STR_PATTERN_Result(
    const regexp::Result_shared  res,
    std::array<std::uint8_t, 4> &ret
);

constexpr regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newGroup(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(1, 4)
                        ->setName("number"),
                    regexp::Pattern::newExactChar(':')
                        ->setMinMaxCount(1, 1)
                   }
               )
                   ->setMinMaxCount(7, 7)
                   ->setName("numbers"),
               regexp::Pattern::newCharIsXDigit()
                   ->setMinMaxCount(1, 4)
                   ->setName("8")
              }

        )
              ->setName("IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN");
    return ret;
}

constexpr regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newGroup(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(1, 4)
                        ->setName("number"),
                    regexp::Pattern::newExactChar(':')
                        ->setMinMaxCount(1, 1)
                   }
               )
                   ->setMinMaxCount(6, 6)
                   ->setName("numbers"),
               IPv4_STR_PATTERN()
              }

        )
              ->setName("IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4");
    return ret;
}

constexpr regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newGroup(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(0, 4)
                        ->setName("number"),
                    regexp::Pattern::newExactChar(':')
                        ->setMinCount(1)
                        ->setMaxCount(2)
                   }
               )
                   ->setMinCount(1)
                   ->setMaxCount(8)
                   ->setName("numbers"),
               regexp::Pattern::newCharIsXDigit()
                   ->setMinMaxCount(0, 4)
                   ->setName("number")
              }
        )
              ->setName("IPv6_SHORT_GRP_HEX_STR_PATTERN");
    return ret;
}

constexpr regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newGroup(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(0, 4)
                        ->setName("number"),
                    regexp::Pattern::newExactChar(':')
                        ->setMinCount(1)
                        ->setMaxCount(2)
                   }
               )
                   ->setMinCount(1)
                   ->setMaxCount(6),
               IPv4_STR_PATTERN()
              }
        )
              ->setName("IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4");
    return ret;
}

constexpr regexp::Pattern_shared IPv6_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newGroup(
              {regexp::Pattern::newExactChar('[')
                   ->setMaxCount(1)
                   ->unsetMinCount(),
               regexp::Pattern::newOrGroup(
                   {/* long 2 byte */
                    IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN(),
                    IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN_COMB_IPv4(),
                    /* short */
                    IPv6_SHORT_GRP_HEX_STR_PATTERN_COMB_IPv4(),
                    IPv6_SHORT_GRP_HEX_STR_PATTERN()
                   }
               ),
               regexp::Pattern::newExactChar(']')
                   ->setMaxCount(1)
                   ->unsetMinCount()
              }
        )
              ->setName("IPv6_STR_PATTERN");
    return ret;
}

error_ptr getNumbersFromShort_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    std::array<std::uint8_t, 16> &ret,
    bool                         &ipv4_comb
);

error_ptr getNumbersFromLong_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    std::array<std::uint8_t, 16> &ret,
    bool                         &ipv4_comb
);

error_ptr getIPBytesFrom_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    std::array<std::uint8_t, 16> &ret,
    bool                         &ipv4_comb
);

constexpr regexp::Pattern_shared IP_STR_PATTERN()
{
    regexp::Pattern_shared ret
        = regexp::Pattern::newOrGroup(
              {IPv4_STR_PATTERN(),
               IPv6_STR_PATTERN()
              }
        )
              ->setExactCount(1)
              ->setName("IP_STR_PATTERN");
    return ret;
}

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

constexpr regexp::Pattern_shared IP_AND_CIDR_OR_PORT_STR_PATTERN(
    IP_AND_CIDR_OR_PORT_STR_PATTERN_mode mode
)
{
    regexp::Pattern_shared ret;
    regexp::Pattern_shared port_or_cidr;

    if (mode == IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_only)
    {
        ret = IP_STR_PATTERN()
                  ->setName("ip");
    }
    else
    {
        switch (mode)
        {
            default:
                throw wayround_i2p::ccutils::errors::New(
                    "invalid mode",
                    __FILE__,
                    __LINE__
                );

            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_cidr_or_port:
                port_or_cidr
                    = regexp::Pattern::newOrGroup(
                          {PORT_STR_PATTERN(),
                           CIDR_STR_PATTERN()
                          }
                    )
                          ->setExactCount(1);
                break;
            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_cidr:
                port_or_cidr = CIDR_STR_PATTERN()->setExactCount(1);
                break;
            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_must_port:
                port_or_cidr = PORT_STR_PATTERN()->setExactCount(1);
                break;

            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_cidr_or_port:
                port_or_cidr
                    = regexp::Pattern::newOrGroup(
                          {PORT_STR_PATTERN(),
                           CIDR_STR_PATTERN()
                          }
                    )
                          ->unsetMinCount()
                          ->setMaxCount(1);
                break;
            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_cidr:
                port_or_cidr = CIDR_STR_PATTERN()->unsetMinCount()->setMaxCount(1);
                break;
            case IP_AND_CIDR_OR_PORT_STR_PATTERN_mode::ip_and_opt_port:
                port_or_cidr = PORT_STR_PATTERN()->unsetMinCount()->setMaxCount(1);
                break;
        }

        ret = regexp::Pattern::newGroup(
            {IP_STR_PATTERN()
                 ->setName("ip"),
             port_or_cidr
                 ->setName("port_or_cidr")
            }
        );
    }

    return ret;
}

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

    void      setFromArray(const std::array<std::uint8_t, 4> &arr);
    error_ptr setFromVector(const std::vector<std::uint8_t> &vec);
    error_ptr setFromString(const UString &val);

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
    static IPv6_shared                        createFromArray(const IPv6_array &arr);
    static std::tuple<IPv6_shared, error_ptr> createFromVector(const std::vector<std::uint8_t> &vec);
    static std::tuple<IPv6_shared, error_ptr> createFromVector(const std::vector<std::uint16_t> &vec);
    static std::tuple<IPv6_shared, error_ptr> createFromVector(const std::vector<std::uint32_t> &vec);
    static std::tuple<IPv6_shared, error_ptr> createFromString(const UString &text);

    void      setFromArray(const std::array<std::uint8_t, 16> &arr);
    void      setFromArray(const std::array<std::uint16_t, 8> &arr);
    void      setFromArray(const std::array<std::uint32_t, 4> &arr);
    void      setFromArray(const IPv6_array &arr);
    error_ptr setFromVector(const std::vector<std::uint8_t> &vec);
    error_ptr setFromVector(const std::vector<std::uint16_t> &vec);
    error_ptr setFromVector(const std::vector<std::uint32_t> &vec);
    error_ptr setFromString(const UString &text);

    IPv6_array                 toArray() const;
    std::vector<std::uint8_t>  toVector8() const;
    std::vector<std::uint16_t> toVector16() const;
    std::vector<std::uint32_t> toVector32() const;
    UString                    toString() const;
    UString                    toStringLong() const;
    UString                    toStringShort() const;

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
