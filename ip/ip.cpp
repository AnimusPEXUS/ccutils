
#include <wayround_i2p/ccutils/ip/ip.hpp>

namespace wayround_i2p::ccutils::ip
{

inline bool check_val_fits_1_byte(long long val)
{
    return (val >= 0 && val <= 255);
}

inline bool check_val_fits_2_bytes(long long val)
{
    return (val >= 0 && val <= 65535);
}

std::tuple<std::uint16_t, error_ptr> getNumberFrom_PORT_STR_PATTERN_Result(
    const regexp::Result_shared res
)
{
    auto err = regexp::ResultRoutineCheck<true, true>(
        res,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return {0, err};
    }

    auto num     = res->findByNameRec("number");
    auto num_str = num->getMatchedString();

    int x = 0;

    try
    {
        x = std::stoi(num_str);
    }
    catch (const std::exception &e)
    {
        return {
            0,
            wayround_i2p::ccutils::errors::New(
                std::format("std::stoi error: {}", e.what()),
                __FILE__,
                __LINE__
            )
        };
    }

    if (!check_val_fits_2_bytes(x))
    {
        return {
            0,
            wayround_i2p::ccutils::errors::New(
                "!res - port value too high",
                __FILE__,
                __LINE__
            )
        };
    }
    return {x, nullptr};
}

std::tuple<std::uint16_t, error_ptr> getNumberFrom_CIDR_STR_PATTERN_Result(
    const regexp::Result_shared res
)
{
    auto err = regexp::ResultRoutineCheck<true, true>(
        res,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return {0, err};
    }

    auto num     = res->findByNameRec("number");
    auto num_str = num->getMatchedString();

    int x = 0;

    try
    {
        x = std::stoi(num_str);
    }
    catch (const std::exception &e)
    {
        return {
            0,
            wayround_i2p::ccutils::errors::New(
                std::format("std::stoi error: {}", e.what()),
                __FILE__,
                __LINE__
            )
        };
    }

    if (!check_val_fits_2_bytes(x))
    {
        return {
            0,
            wayround_i2p::ccutils::errors::New(
                "!res - port value too high",
                __FILE__,
                __LINE__
            )
        };
    }
    return {x, nullptr};
}

error_ptr getIPBytesFrom_IPv4_STR_PATTERN_Result(
    const regexp::Result_shared  res,
    std::array<std::uint8_t, 4> &ret
)
{
    auto err = regexp::ResultRoutineCheck<true, true>(
        res,
        __FILE__,
        __LINE__
    );
    if (err)
    {
        return err;
    }

    for (unsigned char i = 0; i < 4; i++)
    {
        auto res2 = res->findByNameRec(std::to_string(i + 1));

        err = regexp::ResultRoutineCheck<true, true>(
            res2,
            __FILE__,
            __LINE__
        );
        if (err)
        {
            return err;
        }

        auto x = res2->getMatchedString();
        try
        {
            ret[i] = std::stoi(x);
        }
        catch (std::invalid_argument const &ex)
        {
            return wayround_i2p::ccutils::errors::New(
                "invalid_argument",
                __FILE__,
                __LINE__
            );
        }
        catch (std::out_of_range const &ex)
        {
            return wayround_i2p::ccutils::errors::New(
                "out_of_range",
                __FILE__,
                __LINE__
            );
        }
    }
    return nullptr;
}

error_ptr getNumbersFromShort_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    std::array<std::uint8_t, 16> &ret,
    bool                         &ipv4_comb
)
{
    throw wayround_i2p::ccutils::errors::New(
        "todo",
        __FILE__,
        __LINE__
    );
}

error_ptr getNumbersFromLong_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    std::array<std::uint8_t, 16> &ret,
    bool                         &ipv4_comb
)
{
    throw wayround_i2p::ccutils::errors::New(
        "todo",
        __FILE__,
        __LINE__
    );
}

error_ptr getIPBytesFrom_IPv6_STR_PATTERN_Result(
    const regexp::Result_shared   res,
    std::array<std::uint8_t, 16> &ret,
    bool                         &ipv4_comb
)
{
    throw wayround_i2p::ccutils::errors::New(
        "todo",
        __FILE__,
        __LINE__
    );
}

IPv4_shared IPv4::create()
{
    auto ret     = IPv4_shared(new IPv4());
    ret->own_ptr = ret;
    return ret;
}

// see setFromArray() for format explanation
IPv4_shared IPv4::createFromArray(const std::array<std::uint8_t, 4> &arr)
{
    auto ret = IPv4::create();
    ret->setFromArray(arr);
    return ret;
}

// see setFromArray() for format explanation
std::tuple<IPv4_shared, error_ptr> IPv4::createFromVector(const std::vector<std::uint8_t> &vec)
{
    auto ret = IPv4::create();
    auto err = ret->setFromVector(vec);
    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

// see setFromArray() for format explanation
std::tuple<IPv4_shared, error_ptr> IPv4::createFromString(const UString &val)
{
    auto ret = IPv4::create();
    auto err = ret->setFromString(val);
    if (err)
    {
        return {nullptr, err};
    }

    return {ret, nullptr};
}

///
// (in text form, smaller value is on the right)
// 192.168.0.1
//   3   2 1 0
//      |
//      v
// arr[3] arr[2] arr[1] arr[0]
void IPv4::setFromArray(const std::array<std::uint8_t, 4> &arr)
{
    buff = arr;
}

// see setFromArray() for format explanation
error_ptr IPv4::setFromVector(const std::vector<std::uint8_t> &vec)
{
    if (vec.size() != 4)
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid vector size",
            __FILE__,
            __LINE__
        );
    }

    for (std::size_t i = 0; i != 4; i++)
    {
        buff[i] = vec[i];
    }

    return nullptr;
}

// see setFromArray() for format explanation
error_ptr IPv4::setFromString(const UString &val)
{
    std::array<uint8_t, 4> tmp;

    auto pat = IPv4_STR_PATTERN();
    auto res = pat->match(val);

    auto err = getIPBytesFrom_IPv4_STR_PATTERN_Result(res, tmp);
    if (err)
    {
        return err;
    }
    setFromArray(tmp);
    return nullptr;
}

UString IPv4::toString() const
{
    return std::format("{}.{}.{}.{}", buff[3], buff[2], buff[1], buff[0]);
}

std::array<std::uint8_t, 4> IPv4::toArray() const
{
    return buff;
}

std::vector<std::uint8_t> IPv4::toVector() const
{
    std::vector<std::uint8_t> ret(4);
    for (std::size_t i = 0; i != 4; i++)
    {
        ret[i] = buff[i];
    }
    return ret;
}

IPv4::IPv4()
{
}

IPv4::~IPv4()
{
}

IPv6_shared IPv6::create()
{
    auto ret     = IPv6_shared(new IPv6());
    ret->own_ptr = ret;
    return ret;
}

IPv6_shared IPv6::createFromArray(const std::array<std::uint8_t, 16> &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}

IPv6_shared IPv6::createFromArray(const std::array<std::uint16_t, 8> &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}

IPv6_shared IPv6::createFromArray(const IPv6_array &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}

IPv6_shared IPv6::createFromArray(const std::array<std::uint32_t, 4> &arr)
{
    auto ret = IPv6::create();
    ret->setFromArray(arr);
    return ret;
}

std::tuple<IPv6_shared, error_ptr> IPv6::createFromVector(const std::vector<std::uint8_t> &vec)
{
    auto ret = IPv6::create();
    auto err = ret->setFromVector(vec);
    if (err)
    {
        return {nullptr, err};
    }
    return {ret, nullptr};
}

std::tuple<IPv6_shared, error_ptr> IPv6::createFromVector(const std::vector<std::uint16_t> &vec)
{
    auto ret = IPv6::create();
    auto err = ret->setFromVector(vec);
    if (err)
    {
        return {nullptr, err};
    }
    return {ret, nullptr};
}

std::tuple<IPv6_shared, error_ptr> IPv6::createFromVector(const std::vector<std::uint32_t> &vec)
{
    auto ret = IPv6::create();
    auto err = ret->setFromVector(vec);
    if (err)
    {
        return {nullptr, err};
    }
    return {ret, nullptr};
}

std::tuple<IPv6_shared, error_ptr> IPv6::createFromString(const UString &text)
{
    auto ret = IPv6::create();
    auto err = ret->setFromString(text);
    if (err)
    {
        return {nullptr, err};
    }
    return {ret, nullptr};
}

void IPv6::setFromArray(const std::array<std::uint8_t, 16> &arr)
{
    buff.b8 = arr;
}

void IPv6::setFromArray(const std::array<std::uint16_t, 8> &arr)
{
    buff.b16 = arr;
}

void IPv6::setFromArray(const std::array<std::uint32_t, 4> &arr)
{
    buff.b32 = arr;
}

void IPv6::setFromArray(const IPv6_array &arr)
{
    buff = arr;
}

error_ptr IPv6::setFromVector(const std::vector<std::uint8_t> &vec)
{
    if (vec.size() != 16)
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid vector size",
            __FILE__,
            __LINE__
        );
    }

    for (std::size_t i = 0; i != 16; i++)
    {
        buff.b8[i] = vec[i];
    }

    return nullptr;
}

error_ptr IPv6::setFromVector(const std::vector<std::uint16_t> &vec)
{
    if (vec.size() != 8)
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid vector size",
            __FILE__,
            __LINE__
        );
    }

    for (std::size_t i = 0; i != 8; i++)
    {
        buff.b16[i] = vec[i];
    }

    return nullptr;
}

error_ptr IPv6::setFromVector(const std::vector<std::uint32_t> &vec)
{
    if (vec.size() != 4)
    {
        return wayround_i2p::ccutils::errors::New(
            "invalid vector size",
            __FILE__,
            __LINE__
        );
    }

    for (std::size_t i = 0; i != 4; i++)
    {
        buff.b32[i] = vec[i];
    }

    return nullptr;
}

error_ptr IPv6::setFromString(const UString &text)
{
    std::array<std::uint8_t, 16> tmp;

    auto pat = IPv6_STR_PATTERN();
    auto res = pat->match(text);

    auto err = getIPBytesFrom_IPv6_STR_PATTERN_Result(res, tmp, ipv4_comb);
    if (err)
    {
        return err;
    }
    setFromArray(tmp);
    return nullptr;
}

IPv6_array IPv6::toArray() const
{
    return buff;
}

std::vector<std::uint8_t> IPv6::toVector8() const
{
    std::vector<std::uint8_t> ret(16);
    for (std::size_t i = 0; i < 16; i++)
    {
        ret[i] = buff.b8[i];
    }
    return ret;
}

std::vector<std::uint16_t> IPv6::toVector16() const
{
    std::vector<std::uint16_t> ret(8);
    for (std::size_t i = 0; i < 8; i++)
    {
        ret[i] = buff.b16[i];
    }
    return ret;
}

std::vector<std::uint32_t> IPv6::toVector32() const
{
    std::vector<std::uint32_t> ret(4);
    for (std::size_t i = 0; i < 4; i++)
    {
        ret[i] = buff.b32[i];
    }
    return ret;
}

UString IPv6::toString() const
{
    return toStringShort();
}

UString IPv6::toStringLong() const
{
    UString ret;
    for (std::size_t i = 0; i < 8; i++)
    {
        ret += std::format("{:x}", buff.b16[i]);
        if (i < 7)
        {
            ret += ":";
        }
    }
    return ret;
}

UString IPv6::toStringShort() const
{
    struct zeroes_slice
    {
        std::size_t start;
        std::size_t length;
    };

    std::deque<zeroes_slice> slices;

    {
        bool        zeroes_slice_started = false;
        std::size_t zeroes_slice_i       = -1;

        // todo: check this is correct in different endianneses

        for (std::size_t i = 0; i < 8; i++)
        {
            if (buff.b16[i] == 0)
            {
                if (zeroes_slice_started)
                {
                    auto &x = slices[zeroes_slice_i];
                    x.length++;
                }
                else
                {
                    zeroes_slice_i++;
                    slices.push_back(zeroes_slice{i, 1});
                    zeroes_slice_started = true;
                }
            }
            else
            {
                if (zeroes_slice_started)
                {
                    zeroes_slice_started = false;
                }
            }
        }
    }

    if (slices.size() == 0)
    {
        return toStringLong();
    }

    zeroes_slice *longest = &(slices[0]);

    for (std::size_t i = 1; i < slices.size(); i++)
    {
        if (slices[i].length > (*longest).length)
        {
            longest = &(slices[i]);
        }
    }

    zeroes_slice &longest_np = *longest;

    UString ret;

    // todo: check this is correct in different endianneses

    {
        for (
            std::size_t i = 0;
            i < longest_np.start;
            i++
        )
        {
            ret += std::vformat("{:x}", std::make_format_args(buff.b16[i]));
            ret += ":";
        }

        for (
            std::size_t i = longest_np.start + longest_np.length;
            i < 8;
            i++
        )
        {
            ret += ":";
            ret += std::vformat("{:x}", std::make_format_args(buff.b16[i]));
        }
    }
    return ret;
}

void IPv6::setIPv4Comb(bool val)
{
    ipv4_comb = val;
}

void IPv6::setIPv4Comb(const IPv4_shared &comb_part)
{
    auto arr = comb_part->toArray();
    for (std::size_t i = 0; i != 4; i++)
    {
        buff.b8[i + 12] = arr[i];
    }
}

bool IPv6::isIPv4Comb() const
{
    return ipv4_comb;
}

IPv4_shared IPv6::getIPv4Comb() const
{
    std::array<std::uint8_t, 4> arr;
    for (std::size_t i = 0; i != 4; i++)
    {
        arr[i] = buff.b8[i + 12];
    }
    return IPv4::createFromArray(arr);
}

IPv6::IPv6()
{
}

IPv6::~IPv6()
{
}

} // namespace wayround_i2p::ccutils::ip
