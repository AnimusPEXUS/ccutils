#ifndef WAYROUND_I2P_20241007_110017_769194
#define WAYROUND_I2P_20241007_110017_769194

#include <cassert>
#include <format>
#include <functional>
#include <map>
#include <vector>

#include <experimental/scope>

#include <wayround_i2p/ccutils/regexp/regexp_fds_and_enums.hpp>

#include <wayround_i2p/ccutils/utils/concepts.hpp>

#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::regexp
{

// this is wip prototype for UChar (from ccutils/unicode).
// maybe CharT and/or ICU's UChar
// will be added later, but doubt this will be necessary.

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

struct Pattern_repr_as_text_opts
{
    bool    subpatterns = false;
    bool    siblings    = false;
    UString padding     = "  ";

    Pattern_repr_as_text_opts(bool v1)
    {
        subpatterns = v1;
        siblings    = v1;
    }
};

template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T1>
void copyDequeOrVector(
    const T1             &val,
    Pattern_shared_deque &target
)
{
    target.clear();

    for (auto &i : val)
    {
        target.push_back(i);
    }
}

void updateBonds(Pattern_shared_deque &v, Pattern_shared set_parent);

template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T>
Pattern_shared appendPatterns(
    const T       &v,
    Pattern_shared pattern,
    Pattern_shared set_parent
);

struct Pattern : public wayround_i2p::ccutils::repr::RepresentableAsText
{
    // this is used for sequencing

    Pattern_weak   parent;
    Pattern_weak   prev_sibling;
    Pattern_shared next_sibling;

    void setSequenceParent(Pattern_shared val);

    // generate complete deque of current level Patterns
    // (starting from first one)
    Pattern_shared_deque &makeSequenceDeque(Pattern_shared_deque &ret) const;

    // generates deque of all patterns in on curren sequence;
    // appends values from val;
    // updates next/prev siblings
    // returns current Pattern
    template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T>
    Pattern_shared appendPatterns(const T &val)
    {
        ::wayround_i2p::ccutils::regexp::appendPatterns(
            val,
            own_ptr.lock(),
            (parent.expired() ? nullptr : parent.lock())
        );
        return own_ptr.lock();
    }

    std::size_t    findSize() const;
    Pattern_shared findFirst() const;
    Pattern_shared findLast() const;

    UString        name; // can be used to search result sibling by name
    Pattern_shared setName(UString value);

    // if true, result will be placed in special map in search result root,
    // which can be retrieved using Pattern::getShortcutResult(name)
    // bool shortcut_result = false;

    // todo: shortcuts mechanics appeared not as convinient as expected

    // Pattern_shared setShortcutResult(bool value = true);

    PatternType pattern_type;

    // this field used by pattern types requiring exact char values
    // count of values:
    // 1 - for ExactChar,
    // 2 - for CharRange
    // any number of chars for CharList
    std::vector<UChar> values;

    // clears notSubSequence, orGroup and group fields
    void removeAllSubpatterns();

    Pattern_shared       notSubPattern;
    Pattern_shared_deque orGroup;
    Pattern_shared       group;

    bool case_sensitive_from_parent = true;

    // this setting influences following pattern types:
    // ExactChar, CharRange
    bool case_sensitive = true;

    // calculates case-sensitivity basing on
    // case_sensitive_from_parent/case_sensitive values
    bool isCaseSensitive() const;

    bool        has_min = false;
    bool        has_max = false;
    std::size_t min     = 0;
    std::size_t max     = 0;

    // greedy = true = match as less(!) as possible
    bool greedy = false;

    UString repr_as_text() const;
    UString repr_as_text(const Pattern_repr_as_text_opts &opts) const;

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

    Pattern_shared setNot(Pattern_shared notSubPattern);

    Pattern_shared setOrGroup(std::initializer_list<Pattern_shared> val)
    {
        Pattern_shared_deque x;
        for (const auto &i : val)
        {
            x.push_back(i);
        }

        return setOrGroup(x);
    }

    template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T>
    Pattern_shared setOrGroup(const T &val)
    {
        this->pattern_type = PatternType::OrGroup;
        removeAllSubpatterns();
        copyDequeOrVector(val, orGroup);
        auto x = own_ptr.lock();
        for (auto &i : orGroup)
        {
            i->prev_sibling.reset();
            // i->next_sibling = nullptr;
            i->parent = x;
        }
        return own_ptr.lock();
    }

    Pattern_shared setGroup(std::initializer_list<Pattern_shared> val);

    template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T>
    Pattern_shared setGroup(const T &val)
    {
        auto seq = makeSequence(val);
        setGroup(seq);
        return own_ptr.lock();
    }

    Pattern_shared setGroup(Pattern_shared group);

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
    Pattern_shared unsetMinCount();
    Pattern_shared unsetMaxCount();
    Pattern_shared unsetMinMaxCount();

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

    static Pattern_shared newOrGroup(std::initializer_list<Pattern_shared> val);

    template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T>
    static Pattern_shared newOrGroup(const T &val)
    {
        auto ret = Pattern::create();
        ret->setOrGroup(val);
        return ret;
    }

    static Pattern_shared newGroup(std::initializer_list<Pattern_shared> val);

    template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T>
    static Pattern_shared newGroup(const T &val)
    {
        auto ret = Pattern::create();
        ret->setGroup(val);
        return ret;
    }

    static Pattern_shared newGroup(Pattern_shared val);

    static Pattern_shared create();

    const Result_shared match(
        const UString      &subject,
        std::size_t         start_at      = 0,
        const Result_shared parent_result = nullptr
    );

    const Result_shared find(
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

    void minmax_sanity_exception() const;
};

struct Result_repr_as_text_opts
{
    bool    original_subject                  = false;
    bool    corresponding_pattern             = false;
    bool    corresponding_pattern_siblings    = false;
    bool    corresponding_pattern_subpatterns = false;
    bool    subResults                        = false;
    UString padding                           = "  ";

    Result_repr_as_text_opts(bool v1)
    {
        original_subject                  = v1;
        corresponding_pattern             = v1;
        corresponding_pattern_siblings    = v1;
        corresponding_pattern_subpatterns = v1;
        subResults                        = v1;
    }
    Result_repr_as_text_opts(bool v1, bool v2, bool v3, bool v4, bool v5)
    {
        original_subject                  = v1;
        corresponding_pattern             = v2;
        corresponding_pattern_siblings    = v3;
        corresponding_pattern_subpatterns = v4;
        subResults                        = v5;
    }
};

struct Result : public wayround_i2p::ccutils::repr::RepresentableAsText
{
  public:
    Result_weak   parent;
    Result_weak   prev_sibling;
    Result_shared next_sibling;

    std::size_t   findSize() const;
    Result_shared findFirst() const;
    Result_shared findLast() const;

    Result_shared findByIndex(std::size_t index) const;
    Result_shared findByName(UString name, bool rec = false) const;
    Result_shared findByNameRec(UString name) const;
    // same as findByName()
    Result_shared operator[](UString name) const;
    // same as findByIndex()
    Result_shared operator[](std::size_t index) const;

    Result_shared_deque &makeSequenceDeque(Result_shared_deque &ret) const;

    error_ptr error;

    // todo: avoid copying
    UString original_subject;

    bool matched = false;

    // todo: restore this
    std::size_t matched_repetitions_count = 0;

    std::size_t match_start = 0;

    // excludingly. - meaning what character at pos `match_end` - not part
    // of match
    std::size_t match_end = 0;

    Pattern_shared corresponding_pattern;

    // single field enough for Not, OrGroup and SubPattern pattern types
    Result_shared subResult;

    Result_shared getParent();

    // find first pattern of first level sequence
    Result_shared findRoot();

    UString getMatchedString() const;

    UString repr_as_text() const;
    UString repr_as_text(const Result_repr_as_text_opts &opts) const;

    static Result_shared create();

  private:
    std::weak_ptr<Result> own_ptr;
};

// todo: add cancel/abort/limit mesures to functions (or use Contexts)

// ignores repetition and greediness settings and does basic match.
// this is internal function. users should use match() function/
const Result_shared match_single(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at      = 0,
    const Result_shared  parent_result = nullptr
);

// adds repetition and greediness checks layer above match_single.
// optional parent_result is used for backreference expressions.
const Result_shared match(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at      = 0,
    const Result_shared  parent_result = nullptr
);

const Result_shared find(
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

std::tuple<
    bool,  // true = yes
    size_t // length of split (for one of "\r\n", "\n", "\n\r")
    >
    isLineSplit(
        const UString &subject,
        std::size_t    start_at = 0
    );

///
// takes [vector], [deque] or [brace-enclosed initializer list]
// and makes sequence out of [Pattern]s contained in it,
// setting each prev/next siblig field. first element is returned.
template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T>
Pattern_shared makeSequence(const T &val)
{
    if (val.size() == 0)
    {
        throw wayround_i2p::ccutils::errors::New(
            "`val` MUST BE not empty",
            __FILE__,
            __LINE__
        );
    }

    auto                 ret = val[0];
    Pattern_shared_deque x;
    for (std::size_t i = 1; i < val.size(); i++)
    {
        x.push_back(val[i]);
    }
    ret->appendPatterns(x);
    return ret;
}

Pattern_shared makeSequence(std::initializer_list<Pattern_shared> val);

template <wayround_i2p::ccutils::utils::IsDequeOrVectorOfType<Pattern_shared> T>
Pattern_shared appendPatterns(
    const T       &v,
    Pattern_shared pattern,
    Pattern_shared set_parent
)
{
    if (!pattern)
    {
        throw wayround_i2p::ccutils::errors::New(
            "!pattern",
            __FILE__,
            __LINE__
        );
    }

    pattern = pattern->findFirst();

    Pattern_shared_deque x;

    {
        auto current = pattern;
        while (true)
        {
            if (current)
            {
                x.push_back(pattern);
                current = current->next_sibling;
            }
            else
            {
                break;
            }
        }
    }

    for (auto i : v)
    {
        x.push_back(i);
    }

    updateBonds(x, set_parent);

    return pattern;
}

UString PatternTypeString(PatternType v);

} // namespace wayround_i2p::ccutils::regexp

#endif
