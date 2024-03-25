#ifndef WAYROUND_I2P_20240323_153956_245116
#define WAYROUND_I2P_20240323_153956_245116

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <unicode/stringpiece.h>
#include <unicode/unistr.h>
#include <unicode/utypes.h>

// note: no 'ccutils' in namespace.
//       why? - because I feel akigo as separate project but don't
//       think it should really be separate from ccutils.

namespace wayround_i2p::akigo::builtin
{
using size_type   = std::vector<std::uint8_t>::size_type;
using byte_vector = std::vector<std::uint8_t>;

class ustring;

class error
{
    virtual ustring Error() = 0;
};

using error_ptr = std::shared_ptr<error>;

class uchar32
{
  public:
    uchar32();
    ~uchar32();

  private:
    UChar32 chr;
};

class ustring
{
  public:
    ustring();

    ustring(const char *val, std::string encoding = "utf-8");
    ustring(std::string val, std::string encoding = "utf-8");

    ~ustring();

    std::tuple<byte_vector, error> encode(std::string encoding = "utf-8");

    std::string string_utf8();

    uchar32 operator[](std::int32_t offset);

    ustring operator+(ustring &other);
    ustring operator+(std::string &other);

  private:
    icu::UnicodeString data;
};

} // namespace wayround_i2p::akigo::builtin

#endif
