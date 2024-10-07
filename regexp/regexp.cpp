
#include <wayround_i2p/ccutils/regexp/regexp.hpp>

namespace wayround_i2p::ccutils::regexp
{

void copyseq(
    std::initializer_list<Pattern_shared> val,
    Pattern_shared_deque                 &target
)
{
    // target = new Pattern_shared_deque();
    target.clear();

    for (auto &i : val)
    {
        target.push_back(i);
    }
}

Pattern_shared Pattern::setShortcutResult(bool value)
{
    shortcut_result = value;
    return Pattern_shared(this->own_ptr);
}

template <wayround_i2p::ccutils::utils::IsBeginnableSizableIndexaccessable T>
void Pattern::appendToSubpatterns(const T &v)
{
    auto s = v.size();

    // todo: check Pattern type and what it has subpatterns
    for (std::size_t i = 0; i < s; i++)
    {
        subpatterns.push_back(v[i]);
    }
}

error_ptr Pattern::updateParents()
{

    auto current_parent_pattern = own_ptr.lock();
    if (!current_parent_pattern)
    {
        return wayround_i2p::ccutils::errors::New(
            "couldn't get own ptr (nuts!)",
            __FILE__,
            __LINE__
        );
    }

    std::function<void(Pattern_shared)> recSetParent
        = [&recSetParent](Pattern_shared parent)
    {
        for (auto &i : parent->subpatterns)
        {
            i->parent_pattern = parent;
        }

        for (auto &i : parent->subpatterns)
        {
            recSetParent(i);
        }
    };

    return nullptr;
}

bool Pattern::isCaseSensitive() const
{
    if (parent_pattern && case_sensitive_from_parent)
    {
        return parent_pattern->isCaseSensitive();
    }

    return case_sensitive;
}

UString Pattern::repr_as_text() const
{
    return repr_as_text(Pattern_repr_as_text_opts(false));
}

UString Pattern::repr_as_text(const Pattern_repr_as_text_opts &opts) const
{
    // todo: does this return copy? use pointers?
    // todo: todo
    UString ret = std::format(
        R"+++({0:}+---- wayround_i2p::ccutils::regexp::Pattern{{
{0:}|      name                      : {2:}
{0:}|      pattern_type              : {3:}
{0:}|      values                    : {4:}
{0:}|      case_sensitive_from_parent: {5:}
{0:}|      case_sensitive            : {6:}
{0:}|      isCaseSensitive()         : {7:}
{0:}|      has_min                   : {8:}
{0:}|      has_max                   : {9:}
{0:}|      min                       : {10:}
{0:}|      max                       : {11:}
{0:}|      greedy                    : {12:}
{0:}+---- }}{1:})+++",
        opts.padding,
        "\n",
        std::format("\"{}\"", this->name),
        (int)(this->pattern_type),
        "",
        this->case_sensitive_from_parent,
        this->case_sensitive,
        this->isCaseSensitive(),
        this->has_min,
        this->has_max,
        this->min,
        this->max,
        this->greedy
    );

    return ret;
}

Pattern_shared Pattern::setTextStart()
{
    this->pattern_type = PatternType::TextStart;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setTextEnd()
{
    this->pattern_type = PatternType::TextEnd;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setLineStart()
{
    this->pattern_type = PatternType::LineStart;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setLineEnd()
{
    this->pattern_type = PatternType::LineEnd;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setLineSplit()
{
    this->pattern_type = PatternType::LineSplit;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setExactChar(UChar chr)
{
    this->pattern_type = PatternType::ExactChar;
    this->values       = {chr};
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharRange(UChar char0, UChar char1)
{
    if (!(char1 >= char0))
    {
        throw wayround_i2p::ccutils::errors::New(
            "invalid char0/char1 values for Pattern::setCharRange",
            __FILE__,
            __LINE__
        );
    }
    this->pattern_type = PatternType::CharRange;
    this->values       = {char0, char1};
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setAnyChar()
{
    this->pattern_type = PatternType::AnyChar;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharIsAlpha()
{
    this->pattern_type = PatternType::CharIsAlpha;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharIsLower()
{
    this->pattern_type = PatternType::CharIsLower;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharIsUpper()
{
    this->pattern_type = PatternType::CharIsUpper;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharIsPunct()
{
    this->pattern_type = PatternType::CharIsPunct;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharIsDigit()
{
    this->pattern_type = PatternType::CharIsDigit;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharIsXDigit()
{
    this->pattern_type = PatternType::CharIsXDigit;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharIsSpace()
{
    this->pattern_type = PatternType::CharIsSpace;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCharIsBlank()
{
    this->pattern_type = PatternType::CharIsBlank;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setNot(Pattern_shared subpattern)
{
    this->pattern_type = PatternType::Not;
    // this->subpatterns  = Pattern_shared_deque_shared(new Pattern_shared_deque());
    this->subpatterns.push_back(subpattern);
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setOrGroup(std::initializer_list<Pattern_shared> val)
{
    copyseq(val, this->subpatterns);
    this->pattern_type = PatternType::OrGroup;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setSequence(std::initializer_list<Pattern_shared> val)
{
    copyseq(val, this->subpatterns);
    this->pattern_type = PatternType::Sequence;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setName(UString value)
{
    this->name = value;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setRepetition(
    PatternRepetitionType pattern_repetition_type
)
{
    this->has_min = false;
    this->has_max = false;
    this->min     = 0;
    this->max     = 0;

    switch (pattern_repetition_type)
    {
        case PatternRepetitionType::Invalid:
        default:
            throw wayround_i2p::ccutils::errors::New(
                std::format(
                    "invalid pattern_repetition_type value"
                ),
                __FILE__,
                __LINE__
            );
        case PatternRepetitionType::Single:
            setExactCount(1);
            return Pattern_shared(this->own_ptr);

        case PatternRepetitionType::NoneOrOne:
            setMaxCount(1);
            unsetMinCount();
            return Pattern_shared(this->own_ptr);

        case PatternRepetitionType::NoneOrMore:
            unsetMinCount();
            unsetMaxCount();
            return Pattern_shared(this->own_ptr);

        case PatternRepetitionType::OneOrMore:
            setMinCount(1);
            unsetMaxCount();
            return Pattern_shared(this->own_ptr);
    }

    throw wayround_i2p::ccutils::errors::New(
        "unexpected error",
        __FILE__,
        __LINE__
    );
}

Pattern_shared Pattern::setCaseSensitiveFromParent(bool value)
{
    this->case_sensitive_from_parent = value;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setCaseSensitive(bool value)
{
    this->case_sensitive_from_parent = false;
    this->case_sensitive             = value;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setGreedy(bool value)
{
    this->greedy = value;
    return Pattern_shared(this->own_ptr);
}

void Pattern::minmax_sanity_exception() const
{
    if (has_min && has_max)
    {
        if (min > max)
        {
            throw wayround_i2p::ccutils::errors::New(
                "Pattern min > max",
                __FILE__,
                __LINE__
            );
        }
    }
}

Pattern_shared Pattern::setMinCount(std::size_t val)
{
    this->has_min = true;
    this->min     = val;
    // minmax_sanity_exception();
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setMaxCount(std::size_t val)
{
    this->has_max = true;
    this->max     = val;
    // if (!this->has_min)
    // {
    //   setMinCount(0);
    // }
    // minmax_sanity_exception();
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setMinMaxCount(std::size_t min, std::size_t max)
{
    // to avoid exception on min/max check
    this->has_max = false;
    setMinCount(min);
    setMaxCount(max);
    // minmax_sanity_exception();
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setExactCount(std::size_t val)
{
    // to avoid exception on min/max check
    this->has_max = false;
    setMinCount(val);
    setMaxCount(val);
    // minmax_sanity_exception();
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::unsetMinCount()
{
    has_min = false;
    min     = 0;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::unsetMaxCount()
{
    has_max = false;
    max     = 0;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::unsetMinMaxCount()
{
    unsetMinCount();
    unsetMaxCount();
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::newTextStart()
{
    auto ret = Pattern::create();
    ret->setTextStart();
    return ret;
}

Pattern_shared Pattern::newTextEnd()
{
    auto ret = Pattern::create();
    ret->setTextEnd();
    return ret;
}

Pattern_shared Pattern::newLineStart()
{
    auto ret = Pattern::create();
    ret->setLineStart();
    return ret;
}

Pattern_shared Pattern::newLineEnd()
{
    auto ret = Pattern::create();
    ret->setLineEnd();
    return ret;
}

Pattern_shared Pattern::newLineSplit()
{
    auto ret = Pattern::create();
    ret->setLineSplit();
    return ret;
}

Pattern_shared Pattern::newExactChar(UChar chr)
{
    auto ret = Pattern::create();
    ret->setExactChar(chr);
    return ret;
}

Pattern_shared Pattern::newCharRange(UChar char0, UChar char1)
{
    auto ret = Pattern::create();
    ret->setCharRange(char0, char1);
    return ret;
}

Pattern_shared Pattern::newAnyChar()
{
    auto ret = Pattern::create();
    ret->setAnyChar();
    return ret;
}

Pattern_shared Pattern::newCharIsAlpha()
{
    auto ret = Pattern::create();
    ret->setCharIsAlpha();
    return ret;
}

Pattern_shared Pattern::newCharIsLower()
{
    auto ret = Pattern::create();
    ret->setCharIsLower();
    return ret;
}

Pattern_shared Pattern::newCharIsPunct()
{
    auto ret = Pattern::create();
    ret->setCharIsPunct();
    return ret;
}

Pattern_shared Pattern::newCharIsUpper()
{
    auto ret = Pattern::create();
    ret->setCharIsUpper();
    return ret;
}

Pattern_shared Pattern::newCharIsDigit()
{
    auto ret = Pattern::create();
    ret->setCharIsDigit();
    return ret;
}

Pattern_shared Pattern::newCharIsXDigit()
{
    auto ret = Pattern::create();
    ret->setCharIsXDigit();
    return ret;
}

Pattern_shared Pattern::newCharIsSpace()
{
    auto ret = Pattern::create();
    ret->setCharIsSpace();
    return ret;
}

Pattern_shared Pattern::newCharIsBlank()
{
    auto ret = Pattern::create();
    ret->setCharIsBlank();
    return ret;
}

Pattern_shared Pattern::newOrGroup(std::initializer_list<Pattern_shared> val)
{
    auto ret = Pattern::create();
    ret->setOrGroup(val);
    return ret;
}

Pattern_shared Pattern::newSequence(std::initializer_list<Pattern_shared> val)
{
    auto ret = Pattern::create();
    ret->setSequence(val);
    return ret;
}

Pattern_shared Pattern::create()
{
    auto ret     = Pattern_shared(new Pattern());
    ret->own_ptr = ret;
    ret->setRepetition(PatternRepetitionType::Single);
    // ret->setExactCount(1);
    ret->greedy = false;
    return ret;
}

const Result_shared Pattern::match(
    const UString      &subject,
    std::size_t         start_at,
    const Result_shared parent_result
)
{
    return wayround_i2p::ccutils::regexp::match(
        Pattern_shared(this->own_ptr),
        subject,
        start_at,
        parent_result
    );
}

const Result_shared Pattern::search(
    const UString &subject,
    std::size_t    start_at,
    bool           backward
)
{
    return wayround_i2p::ccutils::regexp::search(
        Pattern_shared(this->own_ptr),
        subject,
        start_at,
        backward
    );
}

const std::tuple<
    const Result_shared_deque,
    wayround_i2p::ccutils::errors::error_ptr>
    Pattern::findAll(
        const UString &subject,
        std::size_t    start_at
    )
{
    return wayround_i2p::ccutils::regexp::findAll(
        Pattern_shared(this->own_ptr),
        subject,
        start_at
    );
}

Pattern_shared create()
{
    return Pattern::create();
}

Result_shared Result::getParentResult()
{
    return parent_result;
}

Result_shared Result::getRootResult()
{
    Result_shared ret = Result_shared(own_ptr);
    while (true)
    {
        auto x = ret->getParentResult();
        if (x)
        {
            ret = x;
        }
        else
        {
            break;
        }
    }
    return ret;
}

UString Result::getMatchedString() const
{
    return original_subject.substr(
        this->match_start,
        this->match_end - this->match_start
    );
}

std::size_t Result::getSubmatchCount() const
{
    return submatches.size();
}

Result_shared Result::getSubmatchByPatternName(UString name) const
{
    for (auto i : submatches)
    {
        if (i->corresponding_pattern->name == name)
        {
            return i;
        }
    }
    return nullptr;
}

Result_shared Result::getSubmatchByIndex(std::size_t index) const
{
    return submatches[index];
}

Result_shared Result::operator[](UString name) const
{
    return getSubmatchByPatternName(name);
}

Result_shared Result::operator[](std::size_t index) const
{
    return getSubmatchByIndex(index);
}

Result_shared Result::searchSubmatchByPatternName(UString name) const
{
    for (auto i : submatches)
    {
        if (i->corresponding_pattern->name == name)
        {
            return i;
        }
    }

    for (auto i : submatches)
    {
        auto r = i->searchSubmatchByPatternName(name);
        if (r != nullptr)
        {
            return r;
        }
    }

    return nullptr;
}

Result_shared Result::getShortcutResult(UString name) const
{
    auto res = shortcut_results.find(name);
    if (res == shortcut_results.end())
    {
        return nullptr;
    }
    return res->second;
}

UString Result::repr_as_text() const
{
    return repr_as_text(Result_repr_as_text_opts(false));
}

UString Result::repr_as_text(const Result_repr_as_text_opts &opts) const
{
    UString subm;

    if (opts.submatches)
    {
        if (this->submatches.size() == 0)
        {
            subm = "(none)";
        }
        else
        {
            subm = std::format(
                "(count: {})\n",
                this->submatches.size()
            );

            UString padding = std::format(
                "{}|           ",
                opts.padding
            );

            std::size_t c = 0;
            for (const auto &i : submatches)
            {

                auto cpn  = i->corresponding_pattern->name;
                subm     += std::format(
                    "{}+-- #{} submatch: {}\n",
                    padding,
                    c,
                    (cpn != "" ?
                             std::format("\"{}\"", cpn) :
                             "(unnamed)")
                );
                auto opts2    = opts;
                opts2.padding = std::format(
                    "{}{}      ",
                    padding,
                    (c < this->submatches.size() - 1 ? "|" : " ")
                );
                subm += i->repr_as_text(opts2);
                c++;
            }
        }
    }

    UString corp_patt;

    if (opts.corresponding_pattern)
    {
        if (!this->corresponding_pattern)
        {
            subm = "(absent: and this is wrong!)";
        }
        else
        {
            corp_patt       = "\n";
            UString padding = std::format(
                "{}|           ",
                opts.padding
            );

            auto opts2    = Pattern_repr_as_text_opts(false);
            opts2.padding = std::format(
                "{}|      ",
                padding
            );
            corp_patt += this->corresponding_pattern->repr_as_text(opts2);
        }
    }

    UString ret = std::format(
        R"+++({0:}+---- wayround_i2p::ccutils::regexp::Result{{
{0:}|      error?                    : {2:}
{0:}|      original_subject          : {3:}
{0:}|      matched?                  : {4:}
{0:}|      matched_repetitions_count : {5:}
{0:}|      match_start               : {6:}
{0:}|      match_end                 : {7:}
{0:}|      matched string            : {8:}
{0:}|      corresponding_pattern     : {9:}
{0:}|      submatches                : {10:}
{0:}+---- }}{1:})+++",
        opts.padding,
        "\n",
        this->error ? this->error->Error() : "(no error)",
        opts.original_subject ? this->original_subject : "(disabled)",
        this->matched,
        this->matched_repetitions_count,
        this->match_start,
        this->match_end,
        (this->matched ? std::format("\"{}\"", this->getMatchedString()) : "(dismatched)"),
        (opts.corresponding_pattern ? corp_patt : "(disabled)"),
        (opts.submatches ? subm : "(disabled)")
    );

    return ret;
}

Result_shared Result::create()
{
    auto ret     = Result_shared(new Result());
    ret->own_ptr = ret;
    return ret;
}

bool isTextEnd(
    const UString &subject,
    std::size_t    start_at
)
{
    auto pattern = Pattern::newTextEnd();
    pattern->setRepetition(PatternRepetitionType::Single);

    auto res = match(pattern, subject, start_at);

    return res->matched;
}

bool isLineEnd(
    const UString &subject,
    std::size_t    start_at
)
{
    auto pattern = Pattern::newLineEnd();
    pattern->setRepetition(PatternRepetitionType::Single);

    auto res = match(pattern, subject, start_at);

    return res->matched;
}

std::tuple<
    bool,
    size_t>
    isLineSplit(
        const UString &subject,
        std::size_t    start_at
    )
{
    auto pattern = Pattern::newLineSplit();
    pattern->setRepetition(PatternRepetitionType::Single);

    auto res = match(pattern, subject, start_at);

    return std::tuple(res->matched, res->match_end - res->match_start);
}

// todo: this function is too large and complex. maybe it should be splitup
const Result_shared match_single(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at,
    const Result_shared  parent_result
)
{
    // todo: separate pattern sanity checks to inprove performance?

    auto ret = Result::create();

    auto ex01 = std::experimental::scope_exit(
        [&ret]()
        {
            if (ret->error)
            {
                ret->matched = false;
            }

            if (!ret->matched)
            {
                ret->match_end = ret->match_start;
            }
        }
    );

    ret->original_subject      = subject;
    ret->corresponding_pattern = pattern;
    ret->parent_result         = parent_result;
    ret->match_start           = start_at;
    ret->match_end             = start_at;

    const auto subject_length = subject.length();

    if (start_at > subject_length)
    {
        ret->error
            = wayround_i2p::ccutils::errors::New(
                "trying to work outside of subject",
                __FILE__,
                __LINE__
            );

        return ret;
    }

    if (pattern->pattern_type == PatternType::Invalid)
    {
        ret->error
            = wayround_i2p::ccutils::errors::New(
                "invalid `pattern_type`",
                __FILE__,
                __LINE__
            );

        return ret;
    }

    switch (pattern->pattern_type)
    {
        default:
        {
            ret->error
                = wayround_i2p::ccutils::errors::New(
                    std::format(
                        "unsupported `pattern_type` ({})"
                        " <- programming error : fixme",
                        (unsigned char)pattern->pattern_type
                    ),
                    __FILE__,
                    __LINE__
                );

            return ret;
        }
        case PatternType::Invalid:
        {
            ret->error
                = wayround_i2p::ccutils::errors::New(
                    "invalid `pattern_type`",
                    __FILE__,
                    __LINE__
                );

            return ret;
        }
        case PatternType::TextStart:
        {
            ret->match_end = start_at;
            ret->matched   = start_at == 0;
            return ret;
        }
        case PatternType::TextEnd:
        {
            ret->match_end = start_at;
            ret->matched   = start_at == subject_length;
            return ret;
        }
        case PatternType::LineStart:
        {
            ret->match_start = start_at;
            ret->match_end   = ret->match_start;

            if (start_at == 0)
            {
                ret->matched = true;
                return ret;
            }

            ssize_t pre_substr_length = 0;

            if (start_at > 0)
            {
                pre_substr_length = 1;
            }

            if (start_at > 1)
            {
                pre_substr_length = 2;
            }

            std::tuple<bool, size_t> pre1_match;
            std::tuple<bool, size_t> pre2_match;

            switch (pre_substr_length)
            {
                default:
                {
                    ret->error
                        = wayround_i2p::ccutils::errors::New(
                            "programming error - fixme",
                            __FILE__,
                            __LINE__
                        );
                    return ret;
                }
                case 1:
                {
                    pre1_match = isLineSplit(subject, start_at - 1);
                    if (std::get<0>(pre1_match)
                        && std::get<1>(pre1_match) == 1)
                    {
                        ret->matched = true;
                        return ret;
                    }
                    else
                    {
                        ret->matched = false;
                        return ret;
                    }
                }
                case 2:
                {
                    pre1_match = isLineSplit(subject, start_at - 1);
                    pre2_match = isLineSplit(subject, start_at - 2);
                    if (std::get<0>(pre1_match)
                        && std::get<1>(pre1_match) == 1)
                    {
                        ret->matched = true;
                        return ret;
                    }
                    else
                    {
                        if (std::get<0>(pre2_match)
                            && std::get<1>(pre2_match) == 2)
                        {
                            ret->matched = true;
                            return ret;
                        }
                        else
                        {
                            ret->matched = false;
                            return ret;
                        }
                    }
                }
            }

            return ret;
        }
        case PatternType::LineEnd:
        {
            ret->match_start = start_at;
            ret->matched     = false;
            ret->match_end   = ret->match_start;

            if (start_at == subject_length)
            {
                ret->matched = true;
                return ret;
            }

            auto ils = isLineSplit(subject, start_at);

            if (std::get<0>(ils))
            {
                ret->matched = true;
                return ret;
            }

            return ret;
        }
        case PatternType::LineSplit:
        {
            ret->matched = false;

            auto substr   = subject.substr(start_at, 2);
            auto substr_l = substr.length();

            switch (substr_l)
            {
                default:
                {
                    ret->error
                        = wayround_i2p::ccutils::errors::New(
                            "programming error - fixme",
                            __FILE__,
                            __LINE__
                        );
                    return ret;
                }
                case 0:
                {
                    ret->matched = false;
                    // note: not an error imo. simply missmatch
                    return ret;
                }
                case 1:
                {
                    if (substr == std::vector<UChar>{0x0a})
                    {
                        ret->matched   = true;
                        ret->match_end = ret->match_start + 1;
                        return ret;
                    }
                    else
                    {
                        ret->matched = false;
                        return ret;
                    }
                }
                case 2:
                {
                    if (substr == std::vector<UChar>{0x0d, 0x0a}
                        || substr == std::vector<UChar>{0x0a, 0x0d})
                    {
                        ret->matched   = true;
                        ret->match_end = ret->match_start + 2;
                        return ret;
                    }
                    else
                    {
                        if (substr[0] == 0x0a)
                        {
                            ret->matched   = true;
                            ret->match_end = ret->match_start + 1;
                            return ret;
                        }
                        else
                        {
                            ret->matched = false;
                            return ret;
                        }
                    }
                }
            }

            return ret;
        }
        case PatternType::ExactChar:
        {
            if (pattern->values.size() != 1)
            {
                ret->error
                    = wayround_i2p::ccutils::errors::New(
                        "there must be exactly 1 value inside of pattern->values",
                        __FILE__,
                        __LINE__
                    );
                return ret;
            }

            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            auto target_char = pattern->values[0];

            if (!pattern->isCaseSensitive())
            {
                target_char = target_char.lower();
            }

            if (start_at == subject_length)
            {
                ret->matched = false;
                return ret;
            }

            auto subj_char = subject[start_at];

            if (!pattern->isCaseSensitive())
            {
                subj_char = subj_char.lower();
            }

            ret->matched = target_char == subj_char;

            if (ret->matched)
            {
                ret->match_end = start_at + 1;
            }

            return ret;
        }
        case PatternType::CharRange:
        {
            if (pattern->values.size() != 2)
            {
                ret->error
                    = wayround_i2p::ccutils::errors::New(
                        "there must be exactly 2 values inside of pattern->values",
                        __FILE__,
                        __LINE__
                    );
                return ret;
            }

            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            auto target_char0 = pattern->values[0];
            auto target_char1 = pattern->values[1];

            if (!pattern->isCaseSensitive())
            {
                target_char0 = target_char0.lower();
                target_char1 = target_char1.lower();
            }

            if (target_char0 > target_char1)
            {
                ret->error
                    = wayround_i2p::ccutils::errors::New(
                        "invalid char range specification",
                        __FILE__,
                        __LINE__
                    );
                return ret;
            }

            if (start_at == subject_length)
            {
                ret->matched = false;
                return ret;
            }

            auto subj_char = subject[start_at];

            if (!pattern->isCaseSensitive())
            {
                subj_char = subj_char.lower();
            }

            ret->matched = (target_char0 <= subj_char)
                        && (subj_char < target_char1);

            if (ret->matched)
            {
                ret->match_end = start_at + 1;
            }

            return ret;
        }
        case PatternType::AnyChar:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (auto ls_res = isLineSplit(subject, start_at); std::get<0>(ls_res))
            {
                ret->matched   = true;
                ret->match_end = start_at + std::get<1>(ls_res);
                return ret;
            }

            ret->matched     = true;
            ret->match_start = start_at;
            ret->match_end   = start_at + 1;
            return ret;
        }
        case PatternType::CharIsAlpha:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (subject[start_at].isAlpha())
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = start_at + 1;
                return ret;
            }
            else
            {
                ret->matched = false;
                return ret;
            }
        }
        case PatternType::CharIsLower:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (subject[start_at].isLower())
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = start_at + 1;
                return ret;
            }
            else
            {
                ret->matched = false;
                return ret;
            }
        }
        case PatternType::CharIsUpper:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (subject[start_at].isUpper())
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = start_at + 1;
                return ret;
            }
            else
            {
                ret->matched = false;
                return ret;
            }
        }
        case PatternType::CharIsPunct:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (subject[start_at].isPunct())
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = start_at + 1;
                return ret;
            }
            else
            {
                ret->matched = false;
                return ret;
            }
        }
        case PatternType::CharIsDigit:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (subject[start_at].isDigit())
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = start_at + 1;
                return ret;
            }
            else
            {
                ret->matched = false;
                return ret;
            }
        }
        case PatternType::CharIsXDigit:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (subject[start_at].isXDigit())
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = start_at + 1;
                return ret;
            }
            else
            {
                ret->matched = false;
                return ret;
            }
        }
        case PatternType::CharIsSpace:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (subject[start_at].isSpace())
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = start_at + 1;
                return ret;
            }
            else
            {
                ret->matched = false;
                return ret;
            }
        }
        case PatternType::CharIsBlank:
        {
            if (isLineEnd(subject, start_at))
            {
                ret->matched = false;
                return ret;
            }

            if (subject[start_at].isBlank())
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = start_at + 1;
                return ret;
            }
            else
            {
                ret->matched = false;
                return ret;
            }
        }

        case PatternType::Not:
        {
            if (pattern->subpatterns.size() != 1)
            {
                ret->error = wayround_i2p::ccutils::errors::New(
                    "invalid Pattern for 'Not' PatternType: sequence size() != 1",
                    __FILE__,
                    __LINE__
                );
                return ret;
            }

            auto tmp0 = pattern->subpatterns.operator[](0);
            auto res  = match(tmp0, subject, start_at, ret);

            if (res->error)
            {
                ret->error = res->error;
                return ret;
            }

            // todo: checks required. add checks to tests and put ref here

            if (!res->matched)
            {
                ret->matched = true;

                // note: missmatching means zerolength match is ok,
                // so just leave ret->match_end without change
                //  (which means ret->match_end == ret->match_start)

                // ret->match_end = res->match_end;
                return ret;
            }

            ret->matched = false;
            ret->submatches.push_back(res);
            return ret;
        }

        case PatternType::OrGroup:
        {
            for (auto &x : pattern->subpatterns)
            {
                auto res = match(x, subject, start_at, ret);

                if (res->error)
                {
                    ret->error = res->error;
                    return ret;
                }

                if (res->matched)
                {
                    ret->matched   = true;
                    ret->match_end = res->match_end;
                    ret->submatches.push_back(res);
                    return ret;
                }
            }

            ret->matched = false;
            return ret;
        }

        case PatternType::Sequence:
        {
            std::size_t end_tracker = start_at;

            for (auto &x : pattern->subpatterns)
            {
                auto res = match(x, subject, end_tracker, ret);

                if (res->error)
                {
                    ret->error = res->error;
                    return ret;
                }

                if (res->matched)
                {
                    end_tracker = res->match_end;
                    ret->submatches.push_back(res);
                }
                else
                {
                    ret->matched = false;
                    ret->submatches.clear();
                    return ret;
                }
            }

            ret->matched   = true;
            ret->match_end = end_tracker;

            return ret;
        }
    }

    return ret;
}

const Result_shared match(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at,
    const Result_shared  parent_result
)
{
    Result_shared ret = nullptr;

    std::size_t matched_count = 0;

    auto ex01 = std::experimental::scope_exit(
        [&ret, &matched_count]()
        {
            assert(ret);

            if (ret->error)
            {
                ret->matched = false;
            }

            if (ret->matched)
            {
                ret->matched_repetitions_count = matched_count;
            }
            else
            {
                ret->match_end                 = ret->match_start;
                ret->matched_repetitions_count = 0;
            }
        }
    );

    auto has_min = pattern->has_min;
    auto min     = pattern->min;
    auto has_max = pattern->has_max;
    auto max     = pattern->max;
    auto greedy  = pattern->greedy;

    if (!has_max)
    {
        max = 0;
    }

    if (!has_min)
    {
        min = 0;
    }

    if (has_max && has_min && min > max)
    {
        ret                = Result::create();
        ret->parent_result = parent_result;
        ret->error         = wayround_i2p::ccutils::errors::New(
            "invalid min/max in pattern",
            __FILE__,
            __LINE__
        );
        return ret;
    }

    auto next_start = start_at;
    // auto last_success_mustch_end = start_at;

    while (true)
    {
        auto res = match_single(
            pattern,
            subject,
            next_start,
            parent_result
        );

        if (!ret)
        {
            ret = res;

            if (parent_result && !ret->parent_result)
            {
                throw wayround_i2p::ccutils::errors::New(
                    "parent_result && !ret->parent_result",
                    __FILE__,
                    __LINE__
                );
            }

            if (pattern->shortcut_result && pattern->name != "")
            {
                auto &x = ret->getRootResult()->shortcut_results;
                if (x.find(pattern->name) != x.end())
                {
                    throw wayround_i2p::ccutils::errors::New(
                        std::format(
                            "redefining shortcut result key {}",
                            pattern->name
                        ),
                        __FILE__,
                        __LINE__
                    );
                }
                x[pattern->name] = ret;
            }
        }

        if (res->error)
        {
            ret->error = res->error;
            return ret;
        }

        if (res->matched)
        {
            matched_count++;

            next_start = ret->match_end = res->match_end;

            // todo: detect if greedy but matching
            // global pattern if matching here more than min?
            // pass next pattern to this function and try match-searching?

            if ((greedy)
                && ((min == 0) || (min != 0 && matched_count >= min)))
            {
                goto match_and_exit;
            }

            if (has_max && matched_count == max)
            {
                goto match_and_exit;
            }

            if (has_max && matched_count > max)
            {
                throw wayround_i2p::ccutils::errors::New(
                    "programming error? this should not be happening",
                    __FILE__,
                    __LINE__
                );
            }
        }
        else
        {
            if (has_min)
            {
                if (min != 0)
                {
                    if (matched_count < min)
                    {
                        goto dismatch_and_exit;
                    }
                    else
                    {
                        goto match_and_exit;
                    }
                }
                else
                {
                    goto match_and_exit;
                }
            }
            else
            {
                goto match_and_exit;
            }
        }

        continue;
    match_and_exit:
        ret->matched   = true;
        ret->match_end = next_start;
        return ret;
    dismatch_and_exit:
        ret->matched = false;
        return ret;
    }

    return ret;
}

const Result_shared search(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at,
    bool                 backward
)
{
    // todo: add overlap prevention option?

    Result_shared ret = nullptr;

    auto ex01 = std::experimental::scope_exit(
        [&ret]()
        {
            if (ret)
            {
                if (ret->error)
                {
                    ret->matched = false;
                }

                if (!ret->matched)
                {
                    ret->match_end = ret->match_start;
                }
            }
        }
    );

    auto subject_length = subject.length();

    std::size_t i = start_at;

    while (true)
    {
        if (i >= subject_length)
        {
            break;
        }

        auto match_res = match_single(pattern, subject, i);
        if (match_res->error)
        {
            ret = match_res;
            return ret;
        }

        if (match_res->matched)
        {
            ret = match_res;
            return ret;
        }

        if (backward)
        {
            i--;
        }
        else
        {
            i++;
        };
    }

    return ret;
}

const std::tuple<
    const Result_shared_deque,
    wayround_i2p::ccutils::errors::error_ptr>
    findAll(
        const Pattern_shared pattern,
        const UString       &subject,
        std::size_t          start_at
    )
{
    // todo: add overlap prevention option?

    Result_shared_deque                      ret;
    wayround_i2p::ccutils::errors::error_ptr ret_err = nullptr;

    auto subject_length = subject.length();

    std::size_t i = start_at;

    while (true)
    {
        if (i >= subject_length)
        {
            break;
        }

        auto search_res = search(pattern, subject, i);
        if (!search_res)
        { /* which means: "not found" (which is different from "not matched") */
            break;
        }

        if (search_res->error)
        {
            ret_err = search_res->error;
            return std::tuple(Result_shared_deque{}, ret_err);
        }

        if (search_res->matched)
        {
            ret.push_back(search_res);
            i = search_res->match_end;
            if (search_res->match_end == search_res->match_start)
            {
                i++;
            }
        }
        else
        {
            break;
        }
    }

    return std::tuple(ret, ret_err);
}

} // namespace wayround_i2p::ccutils::regexp
