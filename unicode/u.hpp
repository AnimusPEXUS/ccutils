#ifndef WAYROUND_I2P_20240616_100728_801057
#define WAYROUND_I2P_20240616_100728_801057

#include <cassert>
#include <cstdint>
#include <deque>
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
// #include <wayround_i2p/ccutils/regexp/regexp_enums.hpp>
#include <wayround_i2p/ccutils/regexp/regexp_fds_and_enums.hpp>
// #include <wayround_i2p/ccutils/regexp/regexp.hpp>

#include <wayround_i2p/ccutils/repr/repr.hpp>

namespace wayround_i2p::ccutils::repr
{
class RepresentableAsText;
}

namespace wayround_i2p::ccutils::regexp
{
struct Pattern;
using Pattern_shared        = std::shared_ptr<Pattern>;
using Pattern_shared_vector = std::vector<std::shared_ptr<Pattern>>;
Pattern_shared create();
} // namespace wayround_i2p::ccutils::regexp

namespace wayround_i2p::ccutils::errors
{

class error;
using error_ptr = std::shared_ptr<error>;

} // namespace wayround_i2p::ccutils::errors

namespace wayround_i2p::ccutils::unicode
{
using byte_vector = std::vector<std::uint8_t>;

using error_ptr = wayround_i2p::ccutils::errors::error_ptr;

// class UString;

enum class UCharType : std::uint8_t
{
    None,
    Lower,
    Upper,
    Title,
    Digit,
    Alpha,
    Alnum,
    XDigit,
    Punct,
    Graph,
    Blank,
    Defined,
    Space
};

struct UChar : public wayround_i2p::ccutils::repr::RepresentableAsText
{
  public:
    UChar();
    UChar(std::int32_t val);
    ~UChar();

    std::int32_t as_int32() const;

    UString repr_as_text();

    // friend class UString;

  private:
    UChar32 chr; // todo: can't make this const?
};

bool operator==(
    const UChar &lhs,
    const UChar &rhs
);

bool operator!=(
    const UChar &lhs,
    const UChar &rhs
);

bool operator>(
    const UChar &lhs,
    const UChar &rhs
);

bool operator<(
    const UChar &lhs,
    const UChar &rhs
);

bool operator>=(
    const UChar &lhs,
    const UChar &rhs
);

bool operator<=(
    const UChar &lhs,
    const UChar &rhs
);

class UString : public wayround_i2p::ccutils::repr::RepresentableAsText
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

    // todo: return size in bytes
    // size_t size() const;

    // note: result will be shorter than `length` if
    //       `length` going outside of string
    UString substr(std::size_t pos, std::size_t length) const;

    // split string to lines.
    // result doesn't includes splitting characters
    // (/r, /n and/or they'r combinations).
    // pass existing deque - it will be truncated and new lines will be added
    std::deque<UString> &lines(std::deque<UString> &ret) const;

    std::string string_utf8() const;

    std::vector<UChar> vector_UChar() const;

    UString repr_as_text();

    UChar operator[](std::int32_t offset) const;

    UString operator+(UString &other);

    UString &operator+=(UString &other);
    UString &operator+=(UString &&other);

    operator std::string();

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

std::ostream &operator<<(
    std::ostream &os,
    const UChar  &obj
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
            throw std::format_error("Invalid format args for UString");
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

template <>
struct std::formatter<wayround_i2p::ccutils::unicode::UChar>
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
            throw std::format_error("Invalid format args for UChar");
        }

        return it;
    }

    template <class FmtContext>
    FmtContext::iterator format(
        wayround_i2p::ccutils::unicode::UChar c,
        FmtContext                           &ctx
    ) const
    {
        std::ostringstream out;

        std::vector<wayround_i2p::ccutils::unicode::UChar> v{c};

        wayround_i2p::ccutils::unicode::UString s(v);

        out << s.string_utf8();

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};

#endif
