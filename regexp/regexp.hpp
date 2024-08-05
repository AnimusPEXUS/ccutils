#ifndef WAYROUND_I2P_20240718_170059_658721
#define WAYROUND_I2P_20240718_170059_658721

#include <cassert>
#include <functional>
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

using CBFunctionToCheckChar01 = std::function<
    std::tuple<
        error_ptr,    // return error if error
        bool          // return true if matched ok
        >(UChar,      // here you get char to check
          std::size_t // here you get char offset at subject string
    )>;

// todo: add (thinking required)
//       * 'OnlyIfMatched' pattern option, which forces dismatch if named
//         pattern didn't matched

struct Pattern : public wayround_i2p::ccutils::repr::RepresentableAsText
{
    UString name; // can be used to get submatch by name

    PatternType pattern_type;

    // this field used by pattern types requiring exact char values
    // count of values:
    // 1 - for ExactChar,
    // 2 - for CharRange
    // any number of chars for CharList
    std::vector<UChar> values;

    // number of subpatterns, for:
    //   Not - exactly 1
    //   Sequence - 1 or more
    //   OrSequence - 1 or more
    Pattern_shared_deque_shared subpatterns;

    Pattern_shared parent_pattern;

    // match() function uses this function to update parent_pattern fields in all
    // subpatterns
    error_ptr updateParents();

    bool case_sensitive_from_parent = true;

    // this setting influences following pattern types:
    // ExactChar, CharRange
    bool case_sensitive = true;

    // calculates case-sensitivity basing on
    // case_sensitive_from_parent/case_sensitive values
    bool isCaseSensitive();

    bool        has_min = false;
    bool        has_max = false;
    std::size_t min     = 0;
    std::size_t max     = 0;

    // greedy = true = match as less(!) as possible
    bool greedy = false;

    UString repr_as_text() const;

    Pattern_shared setTextStart();
    Pattern_shared setTextEnd();

    Pattern_shared setLineStart();
    Pattern_shared setLineEnd();

    Pattern_shared setLineSplit();

    Pattern_shared setExactChar(UChar chr);
    Pattern_shared setCharRange(UChar char0, UChar char1);

    Pattern_shared setAnyChar();

    Pattern_shared setCharIsAlpha();
    Pattern_shared setCharIsLower();
    Pattern_shared setCharIsUpper();
    Pattern_shared setCharIsPunct();
    Pattern_shared setCharIsDigit();
    Pattern_shared setCharIsXDigit();
    Pattern_shared setCharIsSpace();
    Pattern_shared setCharIsBlank();

    Pattern_shared setNot(Pattern_shared subpattern);

    Pattern_shared setOrGroup(Pattern_shared_deque_shared seq);
    Pattern_shared setOrGroup(std::initializer_list<Pattern_shared> val);

    Pattern_shared setSequence(Pattern_shared_deque_shared seq);
    Pattern_shared setSequence(std::initializer_list<Pattern_shared> val);

    Pattern_shared setName(UString value);
    Pattern_shared setRepetition(PatternRepetitionType pattern_repetition_type);
    Pattern_shared setCaseSensitiveFromParent(bool value);

    // note: call to this funcion with any value, sets
    // case_sensitive_from_parent to false
    Pattern_shared setCaseSensitive(bool value);
    Pattern_shared setGreedy(bool value = true);

    Pattern_shared setMinCount(std::size_t val);
    Pattern_shared setMaxCount(std::size_t val);
    Pattern_shared setMinMaxCount(std::size_t min, std::size_t max);
    Pattern_shared setExactCount(std::size_t val);

    static Pattern_shared newTextStart();
    static Pattern_shared newTextEnd();

    static Pattern_shared newLineStart();
    static Pattern_shared newLineEnd();

    static Pattern_shared newLineSplit();

    static Pattern_shared newExactChar(UChar chr);
    static Pattern_shared newCharRange(UChar char0, UChar char1);

    static Pattern_shared newAnyChar();

    static Pattern_shared newCharIsAlpha();
    static Pattern_shared newCharIsLower();
    static Pattern_shared newCharIsUpper();
    static Pattern_shared newCharIsPunct();
    static Pattern_shared newCharIsDigit();
    static Pattern_shared newCharIsXDigit();
    static Pattern_shared newCharIsSpace();
    static Pattern_shared newCharIsBlank();

    static Pattern_shared newNot(Pattern_shared subpattern);

    static Pattern_shared newOrGroup(Pattern_shared_deque_shared seq);
    static Pattern_shared newOrGroup(std::initializer_list<Pattern_shared> val);

    static Pattern_shared newSequence(Pattern_shared_deque_shared seq);
    static Pattern_shared newSequence(std::initializer_list<Pattern_shared> val);

    static Pattern_shared create();

    const Result_shared match_single(
        const UString &subject,
        std::size_t    start_at = 0
    );

    const Result_shared match(
        const UString &subject,
        std::size_t    start_at = 0
    );

    const Result_shared search(
        const UString &subject,
        std::size_t    start_at = 0,
        bool           backward = false
    );

    const std::tuple<
        const Result_shared_deque,
        wayround_i2p::ccutils::errors::error_ptr>
        findAll(
            const UString &subject,
            std::size_t    start_at = 0
        );

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
    Result_shared operator[](UString name);

    UString repr_as_text() const;
    UString repr_as_text(const Result_repr_as_text_opts &opts) const;

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

const std::tuple<
    const Result_shared_deque,
    wayround_i2p::ccutils::errors::error_ptr>
    findAll(
        const Pattern_shared pattern,
        const UString       &subject,
        std::size_t          start_at
    );

} // namespace wayround_i2p::ccutils::regexp

#endif
