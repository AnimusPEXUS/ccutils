#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::unicode
{

UChar::UChar(std::int32_t val) :
    chr(val)
{
}

UChar::~UChar()
{
}

UString::UString() :
    data("")
{
}

UString::~UString()
{
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
        vec[i] = val[i].chr;
    }
    //    data = icu::UnicodeString::fromUTF32(reinterpret_cast<int *>(vec.data()));
    data = icu::UnicodeString::fromUTF32(vec.data(), vs);
    return;
}

UString UString::operator+(UString &other)
{
    auto x = data.append(other.data);

    auto z = UString();
    z.data = x;

    return z;
}

UString &UString::operator+=(UString &other)
{
    data = data.append(other.data);
    return *this;
}

UString &UString::operator+=(UString &&other)
{
    data = data.append(other.data);
    return *this;
}

size_t UString::length() const
{
    return this->data.length();
}

/*
size_t UString::size() const
{
    return length();
} */

UString UString::substr(size_t pos, size_t length) const
{
    UString x;
    this->data.extract(pos, length, x.data);
    return x;
}

std::string UString::string_utf8() const
{
    std::string ret;
    ret = data.toUTF8String(ret);
    return ret;
}

UChar UString::operator[](std::int32_t offset)
{
    UChar ret(this->data.char32At(offset));
    return ret;
}

bool operator==(
    const UString &lhs,
    const UString &rhs
)
{
    return lhs.data == rhs.data;
};

UString::operator std::string()
{
    return this->string_utf8();
}

/*
UString::operator const char *()
{
    return this->string_utf8().c_str();
}
*/

bool operator!=(
    const UString &lhs,
    const UString &rhs
)
{
    return !(lhs == rhs);
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

bool operator!=(
    const UString &lhs,
    const char    *rhs
)
{
    return !(lhs == rhs);
};

std::ostream &operator<<(
    std::ostream  &os,
    const UString &obj
)
{
    os << obj.data;
    return os;
}

/*
// todo: this does not work. fix this
// todo: create separate test for this
// testname: xxx_yyy ?
std::ostringstream &operator<<(
    std::ostringstream &os,
    const UString      &obj
)
{
    os.str() = std::string(obj.string_utf8());
    // os << obj.string_utf8();
    // os << obj.string_utf8();
    return os;
}
*/

} // namespace wayround_i2p::ccutils::unicode
