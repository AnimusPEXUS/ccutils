#ifndef WAYROUND_I2P_20240718_170059_650904
#define WAYROUND_I2P_20240718_170059_650904

#include <cassert>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

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

#ifndef CCUTILS_UNICODE_BACKEND
    #error "CCUTILS_UNICODE_BACKEND undefined"
#endif

#if (CCUTILS_UNICODE_BACKEND == icu)
    #include <wayround_i2p/ccutils/unicode_backend_icu/u_backend.hpp>
#elif (CCUTILS_UNICODE_BACKEND == ccutils)
    #include <wayround_i2p/ccutils/unicode_backend_ccutils/u_backend.hpp>
#else
    #error "invalid CCUTILS_UNICODE_BACKEND"
#endif

namespace wayround_i2p::ccutils::unicode
{
using byte_vector = std::vector<std::uint8_t>;

using error_ptr = wayround_i2p::ccutils::errors::error_ptr;

// class UString;

enum class UCharCategory : std::uint32_t
{
    None       = 0,
    Unassigned = 0b01,
    Error      = Unassigned << 1,
    Alpha      = Unassigned << 2,
    Lower      = Unassigned << 3,
    Upper      = Unassigned << 4,
    Punct      = Unassigned << 5,
    Digit      = Unassigned << 6,
    XDigit     = Unassigned << 7,
    Alnum      = Unassigned << 8,
    Space      = Unassigned << 9,
    Blank      = Unassigned << 10,
    Cntrl      = Unassigned << 11,
    Graph      = Unassigned << 12,
    Print      = Unassigned << 13,
};

/*
class UCharPropertiesI
{
  public:
    virtual bool isAlpha()  = 0;
    virtual bool isLower()  = 0;
    virtual bool isUpper()  = 0;
    virtual bool isPunct()  = 0;
    virtual bool isDigit()  = 0;
    virtual bool isXDigit() = 0;
    virtual bool isAlnum()  = 0;
    virtual bool isSpace()  = 0;
    virtual bool isBlank()  = 0;
    virtual bool isCntrl()  = 0;
    virtual bool isGraph()  = 0;
    virtual bool isPrint()  = 0;
};
*/

class UChar : // public UCharPropertiesI,
              public wayround_i2p::ccutils::repr::RepresentableAsText
{
  public:
    UChar();
    UChar(std::int32_t val);

    // to make char from string. string length must be exactly 1, else -> exception
    UChar(const char *std_nullterminated_cstring);
    UChar(std::string stdstring);
    UChar(UString val);

    ~UChar();

    bool checkType(UCharCategory cat) const;

    std::int32_t as_int32() const;

    UString repr_as_text();

    // todo: todo
    //    UCharProperties getAllProperties();

    bool isAlpha() const;
    bool isLower() const;
    bool isUpper() const;
    bool isPunct() const;
    bool isDigit() const;
    bool isXDigit() const;
    bool isAlnum() const;
    bool isSpace() const;
    bool isBlank() const;
    bool isCntrl() const;
    bool isGraph() const;
    bool isPrint() const;

    UChar toLower() const;
    UChar toUpper() const;
    UChar toTitle() const;

    UString propertiesText() const;

  private:
#if (CCUTILS_UNICODE_BACKEND == icu)
    UChar32 chr; // todo: can't make this const?
#endif
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

    UString toLower() const;
    UString toUpper() const;
    UString toTitle() const;

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
#if (CCUTILS_UNICODE_BACKEND == icu)
    icu::UnicodeString data;
#endif
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
