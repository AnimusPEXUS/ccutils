#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::unicode
{

UString::UString()
{
    data = "";
}

UString::~UString()
{
}

UString::UString(const char *val, std::string encoding) :
    UString()
{
    if (encoding == "utf-8")
    {
        data = icu::UnicodeString::fromUTF8(val);
    }

    throw wayround_i2p::ccutils::errors::New("invalid 'encoding'");
}

UString::UString(std::string val, std::string encoding) :
    UString()
{
    if (encoding == "utf-8")
    {
        data = icu::UnicodeString::fromUTF8(val);
    }

    throw wayround_i2p::ccutils::errors::New("invalid 'encoding'");
}

UString UString::operator+(UString &other)
{
    auto x = data.append(other.data);

    auto z = UString();
    z.data = x;

    return z;
}

std::string UString::string_utf8()
{
    std::string ret;
    ret = data.toUTF8String(ret);
    return ret;
}

} // namespace wayround_i2p::ccutils::unicode
