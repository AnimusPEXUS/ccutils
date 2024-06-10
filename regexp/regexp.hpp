#ifndef WAYROUND_I2P_20240601_083133_764006
#define WAYROUND_I2P_20240601_083133_764006

#include <vector>

#include <experimental/scope>

#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::regexp
{

using UChar   = wayround_i2p::ccutils::unicode::UChar;
using UString = wayround_i2p::ccutils::unicode::UString;

// wip prototype for UChar (from ccutils/unicode). maybe CharT and/or ICU's UChar
// will be added later, but doubt this will be necessary.

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
    CharList,

    AnyChar,
    SpecialLetter,
    SpecialName,

    Not,

    Sequence,
    OrSequence
};

enum class PatternRepetitionType : unsigned char
{
    Invalid,
    Single,
    NoneOrOne,
    NoneOrMore,
    OneOrMore,
    ExactCount,
    MinCount,
    MaxCount,
    MinMaxCount
};

// todo: create precompiled patterns

struct Pattern;

using Pattern_shared = std::shared_ptr<Pattern>;

using Pattern_shared_sequence = std::vector<std::shared_ptr<Pattern>>;

struct Pattern
{
    UString name; // can be used to get submatch by name

    PatternType pattern_type;

    // count of values:
    // 1 - for ExactChar,
    // 2 - for CharRange
    // any number of chars for CharList
    std::vector<UChar> values;

    // number of subpatterns, for:
    //   Not - exactly 1
    //   Sequence - 1 or more
    //   OrSequence - 1 or more
    Pattern_shared subpatterns;
    UString        special; // for SpecialLetter or SpecialName
    bool           case_sensitive = true;

    PatternRepetitionType pattern_repetition_type;
    bool                  greedy = false;

    static Pattern_shared create();

  private:
    std::weak_ptr<Pattern> own_ptr;
};

struct Result;

using Result_shared = std::shared_ptr<Result>;

using Result_shared_sequence = std::vector<std::shared_ptr<Result>>;

struct Result
{
    wayround_i2p::ccutils::errors::error_ptr error;

    UString original_subject;

    bool matched;

    std::size_t match_start;

    // excludingly. - meaning what character at pos `match_end` - not part
    // of match
    std::size_t match_end;

    Pattern_shared corresponding_pattern;

    Result_shared_sequence submatches;

    UString       getResultString();
    Result_shared getSubmatchByPatternName(UString name);

    static Result_shared create();

  private:
    std::weak_ptr<Result> own_ptr;
};

std::tuple<
    bool,  // true = yes
    size_t // length of split (for one of "\r\n", "\n", "\n\r")
    >
    isLineSplit(
        const UString &subject,
        std::size_t    start_at = 0
    );

// todo: add cancel/abort/limit mesures to functions

const Result_shared match(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at = 0
);

const Result_shared search(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at = 0,
    bool                 backward = false
);

const std::tuple<
    const Result_shared_sequence,
    wayround_i2p::ccutils::errors::error_ptr>
    findAll(
        const Pattern_shared pattern,
        const UString       &subject,
        std::size_t          start_at = 0
    );

} // namespace wayround_i2p::ccutils::regexp

#endif
