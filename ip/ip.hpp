#ifndef WAYROUND_I2P_20240718_170059_657252
#define WAYROUND_I2P_20240718_170059_657252

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

constexpr regexp::Pattern_shared PORT_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
            {regexp::Pattern::newExactChar(":"),
             regexp::Pattern::newCharIsDigit()
                 ->setName("port_num")
                 ->setMinCount(1)
                 ->unsetMaxCount()
            }
        );
    return ret;
}

constexpr regexp::Pattern_shared CIDR_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
            {regexp::Pattern::newExactChar("/"),
             regexp::Pattern::newCharIsDigit()
                 ->setName("cidr_num")
                 ->setMinCount(1)
                 ->unsetMaxCount()
            }
        );
    return ret;
}

constexpr regexp::Pattern_shared IPv4_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
              {regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("1"),
               regexp::Pattern::newExactChar(".")
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("2"),
               regexp::Pattern::newExactChar(".")
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("3"),
               regexp::Pattern::newExactChar(".")
                   ->setMinMaxCount(1, 1),
               regexp::Pattern::newCharIsDigit()
                   ->setMinMaxCount(1, 3)
                   ->setName("4")
              }
        )
              ->setName("IPv4_STR_PATTERN");
    return ret;
}

constexpr regexp::Pattern_shared IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
              {regexp::Pattern::newSequence(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(1, 4)
                        ->setName("num"),
                    regexp::Pattern::newExactChar(":")
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

constexpr regexp::Pattern_shared IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
              {regexp::Pattern::newSequence(
                   {regexp::Pattern::newCharIsXDigit()
                        ->setMinMaxCount(1, 2)
                        ->setName("num"),
                    regexp::Pattern::newExactChar(":")
                        ->setMinMaxCount(1, 1)}
               )
                   ->setMinMaxCount(15, 15)
                   ->setName("numbers"),
               regexp::Pattern::newCharIsXDigit()
                   ->setMinMaxCount(1, 2)
                   ->setName("15")}
        )
              ->setName("IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN");
    return ret;
}

constexpr regexp::Pattern_shared IPv6_SHORT_GRP_HEX_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
              {
                  regexp::Pattern::newSequence(
                      {regexp::Pattern::newCharIsXDigit()
                           ->setMinMaxCount(0, 4),
                       regexp::Pattern::newExactChar(":")
                      }
                  )
                      ->setMinCount(1)
                      ->setMaxCount(8),
                  regexp::Pattern::newSequence(
                      {regexp::Pattern::newExactChar(":"),
                       regexp::Pattern::newCharIsXDigit()
                           ->setMinMaxCount(0, 4)
                      }
                  )
                      ->setMinCount(1)
                      ->setMaxCount(8),
              }
        )
              ->setName("IPv6_SHORT_GRP_HEX_STR_PATTERN");

    return ret;
}

constexpr regexp::Pattern_shared IPv6_STR_PATTERN()
{
    auto ret
        = regexp::Pattern::newSequence(
              {regexp::Pattern::newExactChar("[")
                   ->setMaxCount(1)
                   ->unsetMinCount(),
               regexp::Pattern::newOrGroup(
                   {/* long 1 byte */
                    IPv6_FULL_1BYTE_GRP_HEX_STR_PATTERN(),
                    /* long 2 byte */
                    IPv6_FULL_2BYTE_GRP_HEX_STR_PATTERN(),
                    /* short */
                    IPv6_SHORT_GRP_HEX_STR_PATTERN()
                   }
               ),
               regexp::Pattern::newExactChar("]")
                   ->setMaxCount(1)
                   ->unsetMinCount()
              }
        )
              ->setName("IPv6_STR_PATTERN");
    return ret;
}

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
                throw wayround_i2p::ccutils::errors::New("invalid mode");

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

        ret = regexp::Pattern::newSequence(
            {IP_STR_PATTERN()
                 ->setName("ip"),
             port_or_cidr
                 ->setName("port_or_cidr")
            }
        );
    }

    return ret;
}

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
