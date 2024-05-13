#ifndef WAYROUND_I2P_20240511_072523_288868
#define WAYROUND_I2P_20240511_072523_288868

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <unicode/stringpiece.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/utypes.h>

// #include <wayround_i2p/ccutils/errors/e.hpp>

namespace wayround_i2p::ccutils::unicode
{
using byte_vector = std::vector<std::uint8_t>;

class error;
using error_ptr = std::shared_ptr<error>;

class UChar
{
  public:
    UChar();
    ~UChar();

  private:
    UChar32 chr;
};

class UString
{
  public:
    UString();

    // todo: make wise copy constructor

    UString(const char *val, std::string encoding = "utf-8");
    UString(std::string val, std::string encoding = "utf-8");

    ~UString();

    std::tuple<byte_vector, error_ptr> encode(std::string encoding = "utf-8");

    size_t length();

    std::string string_utf8();

    UChar operator[](std::int32_t offset);

    UString operator+(UString &other);
    // UString operator+(std::string &other);

    UString &operator+=(UString &other);
    UString &operator+=(UString &&other);

    operator std::string();
    // operator const char *();

    friend bool operator==(
        const UString &lhs,
        const UString &rhs
    );

    friend bool operator==(
        const UString &lhs,
        const char    *rhs
    );

    friend std::ostream &operator<<(std::ostream &os, const UString &obj);

  private:
    icu::UnicodeString data;
};

bool operator==(
    const UString &lhs,
    const UString &rhs
);

bool operator!=(
    const UString &lhs,
    const UString &rhs
);

bool operator==(
    const UString &lhs,
    const char    *rhs
);

bool operator!=(
    const UString &lhs,
    const char    *rhs
);

std::ostream &operator<<(std::ostream &os, const UString &obj);

} // namespace wayround_i2p::ccutils::unicode

#endif
