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

UChar::UChar(char val) :
    UChar((std::int32_t)val)
{
}

// todo: testing required
UChar::UChar(UString val) :
    UChar(
        [val]() -> std::int32_t
        {
            auto vl = val.length();
            if (vl != 1)
            {
                throw wayround_i2p::ccutils::errors::New(
                    std::format("UChar::UChar(UString val) - invalid `val` size: {}", vl)
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

bool UChar::checkType(UCharCategory cat) const
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

UString UChar::repr_as_text() const
{
    return std::format(R"++({:#x})++", chr);
}

UString UChar::propertiesText() const
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

void allcharscheck(
    UString *s,
    bool (UChar::*member)() const,
    bool &x
)
{
    auto l = s->length();
    for (std::size_t i = 0; i < l; i++)
    {
        if (!(((*s)[i]).*member)())
        {
            x = false;
            return;
        }
    }
    x = true;
}

UString::UString() :
    data(""),
    _isAlpha(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isAlpha, x);
            }
        )
    ),
    _isLower(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isLower, x);
            }
        )
    ),
    _isUpper(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isUpper, x);
            }
        )
    ),
    _isPunct(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isPunct, x);
            }
        )
    ),
    _isDigit(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isDigit, x);
            }
        )
    ),
    _isXDigit(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isXDigit, x);
            }
        )
    ),
    _isAlnum(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isAlnum, x);
            }
        )
    ),
    _isSpace(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isSpace, x);
            }
        )
    ),
    _isBlank(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isBlank, x);
            }
        )
    ),
    _isCntrl(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isCntrl, x);
            }
        )
    ),
    _isGraph(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isGraph, x);
            }
        )
    ),
    _isPrint(
        wayround_i2p::ccutils::utils::cached_function::SteadyClockCachedFunction<bool>(
            last_chage_time_point,
            [&](bool &x)
            {
                allcharscheck(this, &UChar::isPrint, x);
            }
        )
    )
{
    update_last_chage_time_point();
}

UString::UString(const UChar &other) :
    UString(std::vector<UChar>{other})
{
}

UString::~UString()
{
}

UString UString::center(
    std::size_t width,
    UChar       fillchar
) const
{
    auto l = length();
    if (width <= l)
    {
        return *this;
    }

    auto diff1      = width - l;
    auto diff1_half = std::size_t(float(diff1) / 2);

    UString new_half_string = "";

    for (std::size_t i = 0; i < diff1_half; i++)
    {
        new_half_string += fillchar;
    }

    UString ret;

    ret = (new_half_string + *this + new_half_string)[0, width];

    while (ret.length() < width) { ret += fillchar; }

    return ret;
}

void UString::setup_default_start_end(
    UString &txt,
    ssize_t &start,
    ssize_t &end
) const
{
    if ((!(end < 0)) && (start < 0))
    {
        throw wayround_i2p::ccutils::errors::New(
            "incorrect start/end. end - set, start - not"
        );
    }

    if (start < 0)
    {
        start = 0;
    }

    if (end < 0)
    {
        end = length();
    }
}

bool UString::is_start_end_correct(
    UString &txt,
    ssize_t  start,
    ssize_t  end
) const
{
    return (
        ((start < 0 && end < 0) || (start >= 0 && end < 0) || (start >= 0 && end >= start))
        && (start < 0 || (start >= 0 && start < length()))
        && (end < 0 || (end >= 0 && end < length()))
    );
}

void UString::exception_on_incorrect_start_end(
    UString &txt,
    ssize_t  start,
    ssize_t  end
) const
{
    if (!is_start_end_correct(txt, start, end))
    {
        throw wayround_i2p::ccutils::errors::New(
            std::format(
                "invalid start({})/end({})/txt.length()({}) combination",
                start,
                end,
                txt.length()
            )
        );
    }
}

bool UString::startswith(
    UString suffix,
    ssize_t start,
    ssize_t end
) const
{
    return false;
}

bool UString::endswith(
    UString suffix,
    ssize_t start,
    ssize_t end
) const
{
    return false;
}

ssize_t UString::index(
    UString sub,
    ssize_t start,
    ssize_t end,
    bool    backwards
) const
{
    exception_on_incorrect_start_end(sub, start, end);

    setup_default_start_end(sub, start, end);

    auto sub_len = sub.length();

    if (sub_len == 0)
    {
        if (!backwards)
        {
            return start;
        }
        else
        {
            return end;
        }
    }

    auto end_min_sub_len = end - sub_len;

    if (end_min_sub_len < 0)
    {
        return -1;
    }

    for (
        ssize_t i = (!backwards ? start : end_min_sub_len);
        (!backwards ? i <= end_min_sub_len : i >= start);
        (!backwards ? i++ : i--)
    )
    {
        for (ssize_t j = 0; j < sub_len; j++)
        {
            if (operator[](i + j) != sub[j])
            {
                goto not_submatched_continue;
            }
        }
        return i;
    not_submatched_continue:
    }
    return -1;
}

ssize_t UString::rindex(
    UString sub,
    ssize_t start,
    ssize_t end
) const
{
    return index(sub, start, end, true);
}

bool UString::isAlpha()
{
    return _isAlpha.getCaching();
}

bool UString::isLower()
{
    return _isLower.getCaching();
}

bool UString::isUpper()
{
    return _isUpper.getCaching();
}

bool UString::isPunct()
{
    return _isPunct.getCaching();
}

bool UString::isDigit()
{
    return _isDigit.getCaching();
}

bool UString::isXDigit()
{
    return _isXDigit.getCaching();
}

bool UString::isAlnum()
{
    return _isAlnum.getCaching();
}

bool UString::isSpace()
{
    return _isSpace.getCaching();
}

bool UString::isBlank()
{
    return _isBlank.getCaching();
}

bool UString::isCntrl()
{
    return _isCntrl.getCaching();
}

bool UString::isGraph()
{
    return _isGraph.getCaching();
}

bool UString::isPrint()
{
    return _isPrint.getCaching();
}

UString UString::lower() const
{
    std::vector<UChar> res;
    for (std::size_t i = 0; i != length(); i++)
    {
        res.push_back(this->operator[](i).lower());
    }
    return UString(res);
}

UString UString::upper() const
{
    std::vector<UChar> res;
    for (std::size_t i = 0; i != length(); i++)
    {
        res.push_back(this->operator[](i).upper());
    }
    return UString(res);
}

UString UString::title() const
{
    // todo: check and fix
    std::vector<UChar> res;
    for (std::size_t i = 0; i != length(); i++)
    {
        res.push_back(this->operator[](i).title());
    }
    return UString(res);
}

std::deque<UString> &UString::splitlines(
    std::deque<UString> &ret,
    bool                 keepends
) const
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

std::string UString::to_string() const
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
        ret[i] = UChar(data.char32At(i));
    }
    return ret;
}

UString UString::repr_as_text() const
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

UChar UString::operator[](ssize_t offset) const
{
    UChar ret(this->data.char32At(offset));
    return ret;
}

UString UString::operator[](ssize_t offset1, ssize_t offset2) const
{
    // todo: add parameters sanity check
    return substr(offset1, offset2 - offset1);
}

UString UString::operator+(const UChar &other) const
{
    return *this + UString(other);
}

UString UString::operator+(const std::string &other) const
{
    return *this + UString(other);
}

UString UString::operator+(const char *other) const
{
    return *this + UString(other);
}

// todo: is this really functional?
/*
UString &UString::operator+=(const UString &&other) const
{
    return this + other;
}

UString &UString::operator+=(const UChar &&other) const
{
    return this + UString(other);
}
*/

void UString::update_last_chage_time_point()
{
    last_chage_time_point = std::chrono::steady_clock::now();
}

UString::operator std::string()
{
    return this->to_string();
}

bool operator!=(
    const UString &lhs,
    const UString &rhs
)
{
    return !(lhs == rhs);
};

bool operator!=(
    const UString &lhs,
    const char    *rhs
)
{
    return !(lhs == rhs);
};

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
    os.str() = std::string(obj.to_string());
    // os << obj.to_string();
    // os << obj.to_string();
    return os;
}
*/

} // namespace wayround_i2p::ccutils::unicode
