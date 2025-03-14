#ifndef WAYROUND_I2P_20250205_113427_114988
#define WAYROUND_I2P_20250205_113427_114988

// todo: create C++ string literal for UString and UChar.

#include <cassert>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <wayround_i2p/ccutils/regexp/regexp_fds_and_enums.hpp>
#include <wayround_i2p/ccutils/repr/repr.hpp>
#include <wayround_i2p/ccutils/utils/cf.hpp>

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

/**
This package is for working with Unicode.

I'm positioning UString/UChar as basic entities to work with text in all
my projects.

this ccutils/uncode is intended to be portable, so it my seamlessly use
local unicode libraries and you not need bother this those libraries
directly.
*/
namespace wayround_i2p::ccutils::unicode
{

using byte_vector = std::vector<std::uint8_t>;

using error_ptr = wayround_i2p::ccutils::errors::error_ptr;

// using ssize_t = std::make_signed_t < std::size_t >> ;

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

/** subpart of UString
 currently it is for storing separate codepoints.
 regardless of actual storage type.

 for instance, with ICU it's actual storage is icu's UChar32 type.
 but you as user of ccutils/unicode should not think about
 it - for you -> single UChar should be equal to single codepoint in your mind.
*/
class UChar : public wayround_i2p::ccutils::repr::RepresentableAsText
{
  public:
    UChar();
    UChar(std::int32_t val);

    UChar(char val);

    // to make char from string. string length must be exactly 1, else -> exception
    UChar(const char *std_nullterminated_cstring);
    UChar(std::string stdstring);
    UChar(UString val);

    ~UChar();

    bool checkType(UCharCategory cat) const;

    /**
     actually you geting codepoint value with this.
     int32 - because, remember, valid unicode codepoints are 0 to 1114111
    */
    std::int32_t as_int32() const;

    UString repr_as_text() const;

    // todo: todo
    //    UCharProperties getAllProperties();

    bool isCased() const;

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

    UChar lower() const;
    UChar upper() const;
    UChar title() const;

    UString propertiesText() const;

  private:
#if (CCUTILS_UNICODE_BACKEND == icu)
    UChar32 chr;
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

/**
 Intention is UString to be as easy and handy to use, as Python 3's string.

 also UString methods are mostly done to be look and feel like Python one's.

 If not specially stated otherwise, behavior different than Python's -
 should be considered as a bug.
*/
class UString : public wayround_i2p::ccutils::repr::RepresentableAsText
{
    // todo: investigate wiser resource usage
    // todo: use `constexpr` in constructors?

  public:
    UString();

    // UString(UString other);

    UString(const UString &other);

    UString(const UChar &other);

    UString(
        const char *val,
        std::string encoding = "utf-8"
    );
    UString(
        const std::string &val,
        std::string        encoding = "utf-8"
    );

    /**
    sometimes it is better to manipulate vectors and/or deques, than UStrings.
    so here are easy UString constructors to fast/easy convert those back to UStrings
    */
    UString(const std::vector<UChar> &val);

    /**
    sometimes it is better to manipulate vectors and/or deques, than UStrings.
    so here are easy UString constructors to fast/easy convert those back to UStrings
    */
    UString(const std::deque<UChar> &val);

    ~UString();

    size_t length() const;

    UString center(
        std::size_t width,
        UChar       fillchar = ' '
    ) const;

    std::size_t count(
        UString sub,
        ssize_t start = -1,
        ssize_t end   = -1
    ) const;

    std::tuple<byte_vector, error_ptr> encode(
        std::string encoding = "utf-8"
    ) const;

    bool startswith(
        UString prefix,
        ssize_t start = -1,
        ssize_t end   = -1
    ) const;

    bool endswith(
        UString suffix,
        ssize_t start = -1,
        ssize_t end   = -1
    ) const;

    UString expandtabs(std::size_t tabsize = 8) const;

    ssize_t index(
        UString sub,
        ssize_t start     = -1,
        ssize_t end       = -1,
        bool    backwards = false
    ) const;

    ssize_t rindex(
        UString sub,
        ssize_t start = -1,
        ssize_t end   = -1
    ) const;

  public:
    bool isAlpha() const;
    bool isLower(bool neglect_caseless_characters = true) const;
    bool isUpper(bool neglect_caseless_characters = true) const;
    bool isPunct() const;
    bool isDigit() const;
    bool isXDigit() const;
    bool isAlnum() const;
    bool isSpace() const;
    bool isBlank() const;
    bool isCntrl() const;
    bool isGraph() const;
    bool isPrint() const;

  public:
    UString capitalize() const;
    // todo: not implemented. is this really neded? not now atleast
    // UString casefold() const;
    UString lower() const;
    UString upper() const;
    UString title() const;
    UString swapcase() const;

    UString lstrip(const std::vector<UChar> &chars) const;
    UString rstrip(const std::vector<UChar> &chars) const;
    UString strip(const std::vector<UChar> &chars) const;

    UString lstrip(UString chars = " ") const;
    UString rstrip(UString chars = " ") const;
    UString strip(UString chars = " ") const;

    std::tuple<
        UString,
        UString,
        UString>
        partition(UString sep) const;

    UString removeprefix(UString prefix) const;
    UString removesuffix(UString suffix) const;

    UString replace(
        UString old_s,
        UString new_s,
        ssize_t count = -1
    ) const;

    UString ljust(std::size_t width, UChar fillchar = ' ') const;
    UString rjust(std::size_t width, UChar fillchar = ' ') const;

    std::deque<UString> &split(
        std::deque<UString> &ret,
        UString              sep,
        ssize_t              maxsplit = -1
    ) const;

    // split string to lines.
    // result doesn't includes splitting characters
    // (/r, /n and/or they'r combinations).
    // pass existing deque - it will be truncated and new lines will be added
    std::deque<UString> &splitlines(
        std::deque<UString> &ret,
        bool                 keepends = false
    ) const;

    // note: result will be shorter than `length` if
    //       `length` going outside of string
    UString substr(std::size_t pos, std::size_t length) const;
    // UString view(std::size_t pos, std::size_t length) const;

    // returns std:string encoded in utf8
    std::string to_string() const;

    std::vector<UChar> to_vector_UChar() const;
    std::deque<UChar>  to_deque_UChar() const;

    UString repr_as_text() const;

    UChar   operator[](ssize_t offset) const;
    UString operator[](ssize_t offset1, ssize_t offset2) const;

    UString operator+(const UString &other) const;
    UString operator+(const UChar &chr) const;
    UString operator+(const std::string &other) const;
    UString operator+(const char *other) const;

    UString &operator+=(const UString &other);
    UString &operator+=(const UChar &other);
    UString &operator+=(const std::string &other);
    UString &operator+=(const char *other);

    // UString &operator+=(const  UString &&other) const;
    // UString &operator+=(const  UChar &&other) const;

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
    // todo: maybe, instead of actual data type, use untyped pointer or std::any here.
    //       to avoid macros usage

#if (CCUTILS_UNICODE_BACKEND == icu)
    icu::UnicodeString data;
    icu::UnicodeString getData() const;
#endif

  private:
    void setup_default_start_end(UString &txt, ssize_t &start, ssize_t &end) const;
    bool is_start_end_correct(UString &txt, ssize_t start, ssize_t end) const;
    void exception_on_incorrect_start_end(UString &txt, ssize_t start, ssize_t end) const;

    // std::chrono::time_point<std::chrono::steady_clock> last_chage_time_point;

    // void update_last_chage_time_point();
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

        out << s.to_string();

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

        out << s.to_string();

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};

#endif
