#include <functional>
#include <vector>

#include <wayround_i2p/ccutils/unicode/u.hpp>

#include <wayround_i2p/ccutils/errors/e.hpp>

#include <wayround_i2p/ccutils/regexp/regexp.hpp>

namespace wayround_i2p::ccutils::unicode
{

UChar::UChar(const char *std_nullterminated_cstring) :
    UChar(UString(std_nullterminated_cstring))
{
}

UChar::UChar(std::string stdstring) :
    UChar(UString(stdstring))
{
}

// todo: testing required
UChar::UChar(UString val) :
    UChar(
        [val]() -> std::int32_t
        {
            if (val.length() != 1)
            {
                throw wayround_i2p::ccutils::errors::New(
                    "UChar::UChar(UString val) - invalid `val` size"
                );
            }

            return val[0].as_int32();
        }()
    )
{
}

UChar::~UChar()
{
}

bool UChar::checkType(UCharCategory cat)
{
    if (cat == UCharCategory::None)
    {
        return true;
    }
    // todo: todo
    return false;
}

std::int32_t UChar::as_int32() const
{
    return chr;
}

UString UChar::repr_as_text()
{
    return std::format(R"++({:#x})++", chr);
}

UString UChar::propertiesText()
{
    UString ret;

    using item01 = std::tuple<UString, std::function<bool()>>;

    /*
        for (auto &i :
    std::vector<item01>{
                 item01(
        "Alpha",
    [&]()->bool{
            return this->isAlpha();
    },
                 item01(
    "Lower",
    [&]()->bool{
            return this->isLower();
    },
             })
        {
            if ((std::get<1>(i))())
            {
                if (ret == "")
                {
            ret += " ";
                }
                ret += std::get<0>(i);
            }
        }
    */

    if (this->isAlpha())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isAlpha";
    }

    if (this->isLower())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isLower";
    }

    if (this->isUpper())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isUpper";
    }

    if (this->isPunct())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isPunct";
    }

    if (this->isDigit())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isDigit";
    }

    if (this->isXDigit())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isXDigit";
    }

    if (this->isAlnum())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isAlnum";
    }

    if (this->isSpace())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isSpace";
    }

    if (this->isCntrl())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isCntrl";
    }

    if (this->isGraph())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isGraph";
    }

    if (this->isPrint())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isPrint";
    }

    return ret;
}

bool operator==(
    const UChar &lhs,
    const UChar &rhs
)
{
    return lhs.as_int32() == rhs.as_int32();
};

bool operator!=(
    const UChar &lhs,
    const UChar &rhs
)
{
    return !(lhs == rhs);
};

bool operator>(
    const UChar &lhs,
    const UChar &rhs
)
{
    return lhs.as_int32() > rhs.as_int32();
}

bool operator<(
    const UChar &lhs,
    const UChar &rhs
)
{
    return lhs.as_int32() < rhs.as_int32();
}

bool operator>=(
    const UChar &lhs,
    const UChar &rhs
)
{
    return lhs.as_int32() >= rhs.as_int32();
}

bool operator<=(
    const UChar &lhs,
    const UChar &rhs
)
{
    return lhs.as_int32() <= rhs.as_int32();
}

UString::UString() :
    data("")
{
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

UString UString::substr(std::size_t pos, std::size_t length) const
{
    UString x;
    this->data.extract(pos, length, x.data);
    return x;
}

std::deque<UString> &UString::lines(std::deque<UString> &ret) const
{
    // std::deque<UString> ret;
    ret.resize(0);

    auto pattern = wayround_i2p::ccutils::regexp::Pattern::newLineSplit();

    /*
    auto pattern = wayround_i2p::ccutils::regexp::Pattern::newLineSplit(
        wayround_i2p::ccutils::regexp::PatternType::LineSplit
    );
*/

    auto search_res = wayround_i2p::ccutils::regexp::findAll(
        pattern,
        *this
    );

    auto search_res_deque      = std::get<0>(search_res);
    auto search_res_err        = std::get<1>(search_res);
    auto search_res_deque_size = search_res_deque.size();

    //	this shouldn't be theoretically happening
    assert(!search_res_err);

    if (search_res_deque_size > 0)
    {
        std::size_t i_start_index = 0;
        std::size_t i_end_index   = 0;

        i_start_index = 0;
        i_end_index   = search_res_deque[0]->match_start;
        // todo: add this check to testsuite and, probably, remove assert
        assert(i_start_index <= i_end_index);
        ret.push_back(
            this->substr(
                i_start_index,
                i_end_index - i_start_index
            )
        );

        if (search_res_deque_size > 1)
        {
            for (std::size_t i = 0; i != (search_res_deque_size - 1); i++)
            {
                i_start_index = search_res_deque[i]
                                    ->match_end;
                i_end_index = search_res_deque[i + 1]
                                  ->match_start;
                assert(i_start_index <= i_end_index);
                ret.push_back(
                    this->substr(
                        i_start_index,
                        i_end_index - i_start_index
                    )
                );
            }
        }

        i_start_index = search_res_deque[search_res_deque_size - 1]->match_end;
        i_end_index   = this->length();
        assert(i_start_index <= i_end_index);
        ret.push_back(
            this->substr(
                i_start_index,
                i_end_index - i_start_index
            )
        );
    }
    else
    {
        ret.push_back(*this);
    }

    return ret;
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

            ret += (this->operator[](i)).repr_as_text();
            ret += ", ";
        }
    }

    if (tl > 0)
    {
        ret += (this->operator[](tl - 1)).repr_as_text();
    }

    ret = "[" + ret + "]";

    return ret;
}

UChar UString::operator[](std::int32_t offset) const
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

std::ostream &operator<<(
    std::ostream &os,
    const UChar  &obj
)
{
    os << std::format("{}", obj);
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
