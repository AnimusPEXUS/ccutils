#ifndef WAYROUND_I2P_20240616_100728_818017
#define WAYROUND_I2P_20240616_100728_818017

#include <cassert>
#include <vector>

#include <experimental/scope>

#include <wayround_i2p/ccutils/regexp/regexp_fds_and_enums.hpp>

#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

// #include <wayround_i2p/ccutils/repr/repr.hpp>

/*
namespace wayround_i2p::ccutils::unicode
{

struct UChar;
class UString;

} // namespace wayround_i2p::ccutils::unicode
*/

/*
namespace wayround_i2p::ccutils::errors
{

class error;
using error_ptr = std::shared_ptr<error>;

} // namespace wayround_i2p::ccutils::errors
*/

namespace wayround_i2p::ccutils::regexp
{

// this is wip prototype for UChar (from ccutils/unicode).
// maybe CharT and/or ICU's UChar
// will be added later, but doubt this will be necessary.

// todo: create precompiled patterns

struct Pattern : public wayround_i2p::ccutils::repr::RepresentableAsText
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

    // for SpecialLetter or SpecialName
    // UString special;
    bool case_sensitive = true;

    bool        has_min = false;
    bool        has_max = false;
    std::size_t min     = 0;
    std::size_t max     = 0;

    // greedy = true = match as less as possible
    bool greedy = false;

    // --vvv-- shortcuts to set min/max fields --vvv--

    error_ptr setRepetitionFromType(
        PatternRepetitionType pattern_repetition_type
    );

    void setMinCount(std::size_t val);
    void setMaxCount(std::size_t val);
    void setMinMaxCount(std::size_t min, std::size_t max);
    void setExactCount(std::size_t val);

    // --^^^-- shortcuts to set min/max fields --^^^--

    UString repr_as_text();

    static Pattern_shared create(PatternType type);

  private:
    std::weak_ptr<Pattern> own_ptr;
};

struct Result_repr_as_text_opts
{
    bool original_subject      = false;
    bool corresponding_pattern = false;
    bool submatches            = false;
};

struct Result : public wayround_i2p::ccutils::repr::RepresentableAsText
{
  public:
    error_ptr error;

    UString original_subject;

    bool matched = false;

    std::size_t matched_repetitions_count = 0;

    std::size_t match_start = 0;

    // excludingly. - meaning what character at pos `match_end` - not part
    // of match
    std::size_t match_end = 0;

    Pattern_shared corresponding_pattern;

    Result_shared_deque submatches;

    UString       getResultString();
    Result_shared getSubmatchByPatternName(UString name);

    UString repr_as_text();
    UString repr_as_text(const Result_repr_as_text_opts &opts);

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

// ignores repetition and greediness settings and does basic match
const Result_shared match_single(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at = 0
);

// adds repetition and greediness checks layer above match_single
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

} // namespace wayround_i2p::ccutils::regexp

#endif
