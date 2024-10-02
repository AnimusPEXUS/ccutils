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
                    std::format(
                        "UChar::UChar(UString val) - invalid `val` size: {}",
                        vl
                    ),
                    __FILE__,
                    __LINE__
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

    if (this->isCased())
    {
        if (ret != "")
        {
            ret += " ";
        }
        ret += "isCased";
    }

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
    const UString *s,
    bool (UChar::*member)() const,
    bool &x
)
{
    auto l = s->length();

    for (std::size_t i = 0; i < l; i++)
    {
        UChar c = (*s)[i];
        if (!((c.*member)()))
        {
            x = false;
            return;
        }
    }
    x = true;
}

void allcasedcharscheck(
    const UString *s,
    bool (UChar::*member)() const,
    bool &x
)
{
    auto l = s->length();

    for (std::size_t i = 0; i < l; i++)
    {
        UChar c = (*s)[i];
        if (!c.isCased())
        {
            continue;
        }

        if (!((c.*member)()))
        {
            x = false;
            return;
        }
    }
    x = true;
}

UString::UString() :
    data("")
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

void UString::setup_default_start_end(
    UString &txt,
    ssize_t &start,
    ssize_t &end
) const
{
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
    auto l = length();
    return (
        (
            (start < 0 && end < 0)
            || (start < 0 && end >= 0)
            || (start >= 0 && end < 0)
            || (start >= 0 && end >= start)
        )
        && (start < 0 || (start >= 0 && start <= l))
        && (end < 0 || (end >= 0 && end <= l))
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
                "invalid length({})"
                "/start({})"
                "/end({})"
                "/txt.length()({}) combination",
                length(),
                start,
                end,
                txt.length()
            ),
            __FILE__,
            __LINE__
        );
    }
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

std::size_t UString::count(
    UString sub,
    ssize_t start,
    ssize_t end
) const
{
    exception_on_incorrect_start_end(sub, start, end);
    setup_default_start_end(sub, start, end);

    std::size_t ret = 0;
    ssize_t     ind = -1;

    ssize_t next_start = start;

    auto sub_len = sub.length();

    while (true)
    {
        ind = index(sub, next_start, end);
        if (ind == -1)
        {
            return ret;
        }

        ret++;

        next_start = ind + sub_len;
    }
}

bool UString::startswith(
    UString prefix,
    ssize_t start,
    ssize_t end
) const
{
    exception_on_incorrect_start_end(prefix, start, end);
    setup_default_start_end(prefix, start, end);

    ssize_t start_plu_prefix_length = start + prefix.length();

    auto res = index(
        prefix,
        start,
        (start_plu_prefix_length > end ? end : start_plu_prefix_length)
    );
    return res == start;
}

bool UString::endswith(
    UString suffix,
    ssize_t start,
    ssize_t end
) const
{
    exception_on_incorrect_start_end(suffix, start, end);
    setup_default_start_end(suffix, start, end);

    auto suff_len         = suffix.length();
    auto end_min_suff_len = end - suff_len;

    if (end_min_suff_len < 0 || end_min_suff_len < start)
    {
        return false;
    }

    auto res = index(suffix, end_min_suff_len, end);
    return res == end_min_suff_len;
}

UString UString::expandtabs(std::size_t tabsize) const
{
    // todo: maybe use std::vector<UChar>. benchmark needed?
    UString tab_string;
    for (std::size_t i = 0; i != tabsize; i++)
    {
        tab_string += " ";
    }

    return replace("\r", tab_string);
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

bool UString::isAlpha() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isAlpha, ret);
    return ret;
}

bool UString::isLower(bool neglect_caseless_characters) const
{
    bool ret = false;
    if (neglect_caseless_characters)
    {
        allcasedcharscheck(this, &UChar::isLower, ret);
    }
    else
    {
        allcharscheck(this, &UChar::isLower, ret);
    }
    return ret;
}

bool UString::isUpper(bool neglect_caseless_characters) const
{
    bool ret = false;
    if (neglect_caseless_characters)
    {
        allcasedcharscheck(this, &UChar::isUpper, ret);
    }
    else
    {
        allcharscheck(this, &UChar::isUpper, ret);
    }
    return ret;
}

bool UString::isPunct() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isPunct, ret);
    return ret;
}

bool UString::isDigit() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isDigit, ret);
    return ret;
}

bool UString::isXDigit() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isXDigit, ret);
    return ret;
}

bool UString::isAlnum() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isAlnum, ret);
    return ret;
}

bool UString::isSpace() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isSpace, ret);
    return ret;
}

bool UString::isBlank() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isBlank, ret);
    return ret;
}

bool UString::isCntrl() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isCntrl, ret);
    return ret;
}

bool UString::isGraph() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isGraph, ret);
    return ret;
}

bool UString::isPrint() const
{
    bool ret = false;
    allcharscheck(this, &UChar::isPrint, ret);
    return ret;
}

UString UString::capitalize() const
{
    auto ret = to_vector_UChar();

    auto l = ret.size();
    if (l == 0)
    {
        return ret;
    }

    ret[0] = ret[0].upper();

    return ret;
}

UString UString::lower() const
{
    std::vector<UChar> res;

    auto l = length();
    for (std::size_t i = 0; i < l; i++)
    {
        res.push_back(this->operator[](i).lower());
    }
    return UString(res);
}

UString UString::upper() const
{
    std::vector<UChar> res;

    auto l = length();
    for (std::size_t i = 0; i < l; i++)
    {
        res.push_back(this->operator[](i).upper());
    }
    return UString(res);
}

UString UString::title() const
{
    // todo: maybe better solution needed
    // todo: this function now works like string.capwords() in Python.
    //       maybe rename it to capwords() and write new function to
    //       be like string.title() in Python
    std::deque<UString> words;
    words = split(words, " ");

    auto l = words.size();

    for (std::size_t i = 0; i < l; i++)
    {
        words[i] = words[i].capitalize();
    }

    UString ret;

    auto l_min_1 = l - 1;

    for (std::size_t i = 0; i < l; i++)
    {
        ret += words[i];
        if (i < l_min_1)
        {
            ret += UChar(' ');
        }
    }

    return ret;
}

UString UString::swapcase() const
{
    std::vector<UChar> ret;

    auto l = length();
    for (std::size_t i = 0; i != l; i++)
    {
        auto x = this->operator[](i);
        ret.push_back(x.isUpper() ? x.lower() : x.upper());
    }
    return ret;
}

UString UString::lstrip(const std::vector<UChar> &chars) const
{
    auto ret = to_deque_UChar();

restart:

    if (ret.size() != 0)
    {
        for (const UChar &i : chars)
        {
            if (ret[0] == i)
            {
                ret.pop_front();
                goto restart;
            }
        }
    }

    return ret;
}

UString UString::rstrip(const std::vector<UChar> &chars) const
{
    auto ret = to_deque_UChar();

restart:

    if (ret.size() != 0)
    {
        for (const UChar &i : chars)
        {
            if (ret[ret.size() - 1] == i)
            {
                ret.pop_back();
                goto restart;
            }
        }
    }

    return ret;
}

UString UString::strip(const std::vector<UChar> &chars) const
{
    return lstrip(chars).rstrip(chars);
}

UString UString::lstrip(UString chars) const
{
    return strip(chars.to_vector_UChar());
}

UString UString::rstrip(UString chars) const
{
    return strip(chars.to_vector_UChar());
}

UString UString::strip(UString chars) const
{
    return strip(chars.to_vector_UChar());
}

std::tuple<
    UString,
    UString,
    UString>
    UString::partition(UString sep) const
{
    auto pos = index(sep);
    if (pos == -1)
    {
        return {*this, "", ""};
    }
    else
    {
        return {
            operator[](0, pos),
            sep,
            operator[](pos + sep.length(), length())
        };
    }
}

UString UString::removeprefix(UString prefix) const
{
    if (startswith(prefix))
    {
        return operator[](prefix.length(), length());
    }
    else
    {
        return *this;
    }
}

UString UString::removesuffix(UString suffix) const
{
    if (endswith(suffix))
    {
        return operator[](0, length() - suffix.length());
    }
    else
    {
        return *this;
    }
}

UString UString::replace(
    UString old_s,
    UString new_s,
    ssize_t count
) const
{
    if (count < -1)
    {
        count = -1;
    }

    UString ret;

    std::size_t old_s_len = old_s.length();

    ssize_t countdown = count;

    std::size_t next_slice_start = 0;

    while (true)
    {
        if (count != -1 && countdown < 0)
        {
            break;
        }

        ssize_t ind = index(old_s, next_slice_start);
        if (ind == -1)
        {
            break;
        }

        ret += operator[](next_slice_start, ind);
        ret += new_s;

        next_slice_start = ind + old_s_len;

        if (count != -1)
        {
            countdown--;
        }
    }

    ret += operator[](next_slice_start, length());

    return ret;
}

UString UString::ljust(std::size_t width, UChar fillchar) const
{
    auto l = length();

    if (l >= width)
    {
        return *this;
    }

    auto ret = to_deque_UChar();

    while (ret.size() < width)
    {
        ret.push_back(fillchar);
    }

    return ret;
}

UString UString::rjust(std::size_t width, UChar fillchar) const
{
    auto l = length();

    if (l >= width)
    {
        return *this;
    }

    auto ret = to_deque_UChar();

    while (ret.size() < width)
    {
        ret.push_front(fillchar);
    }

    return ret;
}

std::deque<UString> &UString::split(
    std::deque<UString> &ret,
    UString              sep,
    ssize_t              maxsplit
) const
{
    if (maxsplit < 0)
    {
        maxsplit = -1;
    }

    // todo: maxsplit doesn't work

    ret.resize(0);

    std::size_t sep_len = sep.length();

    ssize_t cur_end = length();

    while (true)
    {
        auto ind = rindex(sep, 0, cur_end);
        if (ind == -1)
        {
            ret.push_front(operator[](0, cur_end));
            return ret;
        }

        ret.push_front(operator[](ind + sep_len, cur_end));
        cur_end = ind;
    }
}

std::deque<UString> &UString::splitlines(
    std::deque<UString> &ret,
    bool                 keepends
) const
{
    ret.resize(0);

    auto pattern = wayround_i2p::ccutils::regexp::Pattern::newLineSplit();

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

std::vector<UChar> UString::to_vector_UChar() const
{
    auto               tl = this->length();
    std::vector<UChar> ret(tl);
    for (size_t i = 0; i != tl; i++)
    {
        ret[i] = UChar(data.char32At(i));
    }
    return ret;
}

std::deque<UChar> UString::to_deque_UChar() const
{
    auto              tl = this->length();
    std::deque<UChar> ret(tl);
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
    if (offset < 0)
    {
        offset = length() + offset;
    }
    UChar ret(this->data.char32At(offset));
    return ret;
}

UString UString::operator[](ssize_t offset1, ssize_t offset2) const
{
    // todo: add parameters sanity check
    if (offset1 < 0)
    {
        offset1 = length() + offset1;
    }
    if (offset2 < 0)
    {
        offset2 = length() + offset2;
    }
    if (offset1 > offset2)
    {
        offset2 = offset1;
    }
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
