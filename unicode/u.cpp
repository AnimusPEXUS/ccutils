#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

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

UChar::~UChar()
{
}

std::int32_t UChar::as_int32() const
{
    return chr;
}

UString UChar::repr_as_text()
{
    return std::format(R"++(\x{:#04x})++", chr);
}

UString::UString() :
    data("")
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
        vec[i] = val[i].as_int32();
    }
    //    data = icu::UnicodeString::fromUTF32(reinterpret_cast<int *>(vec.data()));
    data = icu::UnicodeString::fromUTF32(vec.data(), vs);
    return;
}

UString::~UString()
{
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

std::vector<UChar> UString::vector_UChar() const
{
    auto               tl = this->length();
    std::vector<UChar> ret(tl);
    for (size_t i = 0; i != tl; i++)
    {
        // (reinterpret_cast<UChar *>(ret.data()))[i] = UChar(data.char32At(i));
        // ret[i] = UChar(reinterpret_cast<std::int32_t>(data.char32At(i)));
        ret[i] = UChar(data.char32At(i));
    }
    return ret;
}

UString UString::repr_as_text()
{
    std::string ret;

    auto tl = this->length();

    if (tl > 1)
    {
        for (size_t i = 0; i != tl - 1; i++)
        {
            auto x  = this[i].repr_as_text();
            ret    += x;
            ret    += ", ";
        }
    }

    ret = "[" + ret + "]";

    return ret;
}

UChar UString::operator[](std::int32_t offset)
{
    UChar ret(this->data.char32At(offset));
    return ret;
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

// todo: is this really functional
UString &UString::operator+=(UString &&other)
{
    data = data.append(other.data);
    return *this;
}

UString::operator std::string()
{
    return this->string_utf8();
}

bool operator==(
    const UString &lhs,
    const UString &rhs
)
{
    return lhs.data == rhs.data;
};

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
