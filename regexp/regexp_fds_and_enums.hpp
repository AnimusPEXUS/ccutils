#ifndef WAYROUND_I2P_20260131_182305_0000234835
#define WAYROUND_I2P_20260131_182305_0000234835

#include <deque>
#include <memory>
#include <vector>

namespace wayround_i2p::ccutils::unicode
{

struct UChar;
class UString;

} // namespace wayround_i2p::ccutils::unicode

namespace wayround_i2p::ccutils::errors
{

class error;
using error_ptr = std::shared_ptr<error>;

} // namespace wayround_i2p::ccutils::errors

namespace wayround_i2p::ccutils::regexp
{

using UChar     = wayround_i2p::ccutils::unicode::UChar;
using UString   = wayround_i2p::ccutils::unicode::UString;
using error_ptr = wayround_i2p::ccutils::errors::error_ptr;

struct Pattern;
using Pattern_shared           = std::shared_ptr<Pattern>;
using Pattern_shared_reference = Pattern_shared &;
using Pattern_weak             = std::weak_ptr<Pattern>;
using Pattern_shared_vector    = std::vector<Pattern_shared>;
using Pattern_shared_deque     = std::deque<Pattern_shared>;

struct Result;
using Result_shared        = std::shared_ptr<Result>;
using Result_weak          = std::weak_ptr<Result>;
using Result_shared_vector = std::vector<Result_shared>;
using Result_shared_deque  = std::deque<Result_shared>;

enum class PatternType : unsigned char
{
    Invalid,

    TextStart,
    TextEnd,
    LineStart,
    LineEnd,

    LineSplit,

    ExactChar,
    CharRange,

    CharValidFunc,

    AnyChar,

    CharIsAlpha,
    CharIsLower,
    CharIsUpper,
    CharIsPunct,
    CharIsDigit,
    CharIsXDigit,
    CharIsSpace,
    CharIsBlank,

    Not,

    OrGroup,
    Group
};

enum class PatternRepetitionType : unsigned char
{
    Invalid,
    Single,
    NoneOrOne,  // ?
    NoneOrMore, // *
    OneOrMore   // +
};

const std::tuple<
    const Result_shared_deque,
    wayround_i2p::ccutils::errors::error_ptr>
    findAll(
        const Pattern_shared     pattern,
        std::shared_ptr<UString> subject,
        std::size_t              start_at = 0
    );

const std::tuple<
    const Result_shared_deque,
    wayround_i2p::ccutils::errors::error_ptr>
    findAll(
        const Pattern_shared pattern,
        const UString       &subject,
        std::size_t          start_at = 0
    );

// Pattern_shared create(PatternType type);

} // namespace wayround_i2p::ccutils::regexp

#endif
