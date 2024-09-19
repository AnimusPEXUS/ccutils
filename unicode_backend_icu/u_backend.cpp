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

UChar UChar::lower() const
{
    return u_tolower(this->chr);
}

UChar UChar::upper() const
{
    return u_toupper(this->chr);
}

UChar UChar::title() const
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

size_t UString::length() const
{
    return this->data.length();
}

UString UString::substr(std::size_t pos, std::size_t length) const
{
    UString x;
    this->data.extract(pos, length, x.data);
    return x;
}

UString UString::operator+(const UString &other) const
{
    // todo: optimizations and improvements required here

    auto od = other.data;
    auto td = data;

    auto x = td.append(od);

    auto z = UString();
    z.data = x;

    return z;
}

UString &UString::operator+=(const UString &other)
{
    data = data.append(other.data);
    return *this;
}

UString &UString::operator+=(const UChar &other)
{
    data = data.append(UString(other).data);
    return *this;
}

UString &UString::operator+=(const std::string &other)
{
    data = data.append(UString(other).data);
    return *this;
}

UString &UString::operator+=(const char *other)
{
    data = data.append(UString(other).data);
    return *this;
}

bool operator==(
    const UString &lhs,
    const UString &rhs
)
{
    return lhs.data == rhs.data;
};

bool operator<(
    const UString &lhs,
    const UString &rhs
)
{
    return lhs.data < rhs.data;
}

bool operator==(
    const UString &lhs,
    const char    *rhs
)
{
    return lhs.data == UString(rhs).data;
};

std::ostream &operator<<(
    std::ostream  &os,
    const UString &obj
)
{
    os << obj.data;
    return os;
}

} // namespace wayround_i2p::ccutils::unicode
