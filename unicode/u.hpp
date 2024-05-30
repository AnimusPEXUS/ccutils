#ifndef WAYROUND_I2P_20240527_064240_395562
#define WAYROUND_I2P_20240527_064240_395562

#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
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

// class UString;

struct UChar
{
  public:
    UChar(std::int32_t val);
    ~UChar();

    friend class UString;

  private:
    const UChar32 chr;
};

class UString
{
    // todo: investigate wiser resource usage
    // todo: use `constexpr` in constructors?

  public:
    UString();

    UString(
        const char *val,
        std::string encoding = "utf-8"
    );

    UString(
        const std::string &val,
        std::string        encoding = "utf-8"
    );

    UString(
        const std::vector<UChar> &val
    );

    ~UString();

    std::tuple<byte_vector, error_ptr> encode(std::string encoding = "utf-8");

    size_t length() const;
    // size_t size() const;

    // note: result will be shorter than `length` if
    //       `length` going outside of string
    UString substr(size_t pos, size_t length) const;

    std::string string_utf8() const;

    UChar operator[](std::int32_t offset);

    UString operator+(UString &other);
    // UString operator+(std::string &other);

    UString &operator+=(UString &other);
    UString &operator+=(UString &&other);

    operator std::string();
    // operator const char *();

    friend bool operator==(
        const UString &lhs,
        const UString
            &rhs
    );

    friend bool operator<(
        const UString &lhs,
        const UString &rhs
    );

    friend bool operator==(
        const UString &lhs,
        const char    *rhs
    );

    friend std::ostream &operator<<(
        std::ostream &os,
        const UString
            &obj
    );

    /*
    friend std::ostringstream &operator<<(
        std::ostringstream &os,
        const UString      &obj
    );
*/

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

bool operator<(
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

std::ostream &operator<<(
    std::ostream  &os,
    const UString &obj
);

/*
std::ostringstream &operator<<(
    std::ostringstream &os,
    const UString      &obj
);*/

} // namespace wayround_i2p::ccutils::unicode

template <>
struct std::formatter<wayround_i2p::ccutils::unicode::UString>
{
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext &ctx)
    {
        auto it = ctx.begin();

        if (it == ctx.end())
        {
            return it;
        }

        if (it != ctx.end() && *it != '}')
        {
            throw std::format_error("Invalid format args for UString.");
        }

        return it;
    }

    template <class FmtContext>
    FmtContext::iterator format(
        wayround_i2p::ccutils::unicode::UString s,
        FmtContext                             &ctx
    ) const
    {
        std::ostringstream out;

        out << s.string_utf8();

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};

#endif
