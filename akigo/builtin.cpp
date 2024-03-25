#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/errors.hpp>

namespace wayround_i2p::akigo::builtin
{

ustring::ustring()
{
    data = "";
}

ustring::~ustring()
{
}

ustring::ustring(const char *val, std::string encoding) :
    ustring()
{
    if (encoding == "utf-8")
    {
        data = icu::UnicodeString::fromUTF8(val);
    }

    throw wayround_i2p::akigo::errors::New("invalid 'encoding'");
}

ustring::ustring(std::string val, std::string encoding) :
    ustring()
{
    if (encoding == "utf-8")
    {
        data = icu::UnicodeString::fromUTF8(val);
    }

    throw wayround_i2p::akigo::errors::New("invalid 'encoding'");
}

ustring ustring::operator+(ustring &other)
{
    auto x = data.append(other.data);

    auto z = ustring();
    z.data = x;

    return z;
}

std::string ustring::string_utf8()
{
    std::string ret;
    ret = data.toUTF8String(ret);
    return ret;
}

} // namespace wayround_i2p::akigo::builtin
