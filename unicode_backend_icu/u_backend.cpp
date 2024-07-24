#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>
#include <wayround_i2p/ccutils/unicode_backend_icu/u_backend.hpp>

namespace wayround_i2p::ccutils::unicode
{

UChar::UChar() :
    chr(0)
{
}

UChar::UChar(std::int32_t val) :
    chr(val)
{
}

bool UChar::isAlpha() const
{
    return u_isalpha(this->chr);
}

bool UChar::isLower() const
{
    return u_islower(this->chr);
}

bool UChar::isUpper() const
{
    return u_isupper(this->chr);
}

bool UChar::isPunct() const
{
    return u_ispunct(this->chr);
}

bool UChar::isDigit() const
{
    return u_isdigit(this->chr);
}

bool UChar::isXDigit() const
{
    return u_isxdigit(this->chr);
}

bool UChar::isAlnum() const
{
    return u_isalnum(this->chr);
}

bool UChar::isSpace() const
{
    return u_isspace(this->chr);
}

bool UChar::isBlank() const
{
    return u_isblank(this->chr);
}

bool UChar::isCntrl() const
{
    return u_iscntrl(this->chr);
}

bool UChar::isGraph() const
{
    return u_isgraph(this->chr);
}

bool UChar::isPrint() const
{
    return u_isprint(this->chr);
}

UChar UChar::toLower() const
{
    return u_tolower(this->chr);
}

UChar UChar::toUpper() const
{
    return u_toupper(this->chr);
}

UChar UChar::toTitle() const
{
    return u_totitle(this->chr);
}

UString::UString(
    const char *val,
    std::string encoding
) :
    UString()
{
    if (encoding == "utf-8")
    {
        data = icu::UnicodeString::fromUTF8(val);
        return;
    }

    throw wayround_i2p::ccutils::errors::New("invalid 'encoding'");
}

UString::UString(
    const std::string &val,
    std::string        encoding
) :
    UString()
{
    if (encoding == "utf-8")
    {
        data = icu::UnicodeString::fromUTF8(val);
        return;
    }

    throw wayround_i2p::ccutils::errors::New("invalid 'encoding'");
}

UString::UString(
    const std::vector<UChar> &val
) :
    UString()
{
    auto vs = val.size();

    std::vector<int32_t> vec(vs);
    for (size_t i = 0; i != vs; i++)
    {
        vec[i] = val[i].as_int32();
    }
    //    data = icu::UnicodeString::fromUTF32(reinterpret_cast<int *>(vec.data()));
    data = icu::UnicodeString::fromUTF32(vec.data(), vs);
    return;
}

} // namespace wayround_i2p::ccutils::unicode
