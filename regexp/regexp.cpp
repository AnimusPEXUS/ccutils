
#include <wayround_i2p/ccutils/regexp/regexp.hpp>

namespace wayround_i2p::ccutils::regexp
{

UString Pattern::repr_as_text()
{
    // todo: does this return copy? use pointers?
    // todo: todo
    UString ret = std::format(
        R"+++(todo)+++"
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
            "invalid char0/char1 values for Pattern::setCharRange"
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

Pattern_shared Pattern::setSequence(Pattern_shared_deque seq)
{
    this->pattern_type = PatternType::Sequence;
    this->subpatterns  = seq;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setOrSequence(Pattern_shared_deque seq)
{
    this->pattern_type = PatternType::OrSequence;
    this->subpatterns  = seq;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setName(UString value)
{
    this->name = value;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setRepetitionFromType(PatternRepetitionType pattern_repetition_type)
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
                    "invalid pattern_repetition_type value. {}:{}",
                    __FILE__,
                    __LINE__
                )
            );
        case PatternRepetitionType::Single:
            this->has_min = true;
            this->has_max = true;
            this->min     = 1;
            this->max     = 1;
            return Pattern_shared(this->own_ptr);

        case PatternRepetitionType::NoneOrOne:
            this->has_min = true;
            this->has_max = true;
            // this->min     = 0;
            this->max     = 1;
            return Pattern_shared(this->own_ptr);

        case PatternRepetitionType::NoneOrMore:
            this->has_min = true;
            // this->has_max = false;
            // this->min     = 0;
            // this->max     = 0;
            return Pattern_shared(this->own_ptr);

        case PatternRepetitionType::OneOrMore:
            this->has_min = true;
            // this->has_max = false;
            this->min     = 1;
            // this->max     = 0;
            return Pattern_shared(this->own_ptr);
    }

    throw wayround_i2p::ccutils::errors::New(
        std::format(
            "unexpected error. {}:{}",
            __FILE__,
            __LINE__
        )
    );
}

Pattern_shared Pattern::setGreedy(bool value)
{
    this->greedy = true;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setMinCount(std::size_t val)
{
    this->has_min = true;
    this->min     = val;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setMaxCount(std::size_t val)
{
    this->has_max = true;
    this->max     = val;
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setMinMaxCount(std::size_t min, std::size_t max)
{
    setMinCount(min);
    setMaxCount(max);
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::setExactCount(std::size_t val)
{
    setMinCount(val);
    setMaxCount(val);
    return Pattern_shared(this->own_ptr);
}

Pattern_shared Pattern::TextStart()
{
    auto ret = Pattern::create();
    ret->setTextStart();
    return ret;
}

Pattern_shared Pattern::TextEnd()
{
    auto ret = Pattern::create();
    ret->setTextEnd();
    return ret;
}

Pattern_shared Pattern::LineStart()
{
    auto ret = Pattern::create();
    ret->setLineStart();
    return ret;
}

Pattern_shared Pattern::LineEnd()
{
    auto ret = Pattern::create();
    ret->setLineEnd();
    return ret;
}

Pattern_shared Pattern::LineSplit()
{
    auto ret = Pattern::create();
    ret->setLineSplit();
    return ret;
}

Pattern_shared Pattern::ExactChar(UChar chr)
{
    auto ret = Pattern::create();
    ret->setExactChar(chr);
    return ret;
}

Pattern_shared Pattern::CharRange(UChar char0, UChar char1)
{
    auto ret = Pattern::create();
    ret->setCharRange(char0, char1);
    return ret;
}

Pattern_shared Pattern::AnyChar()
{
    auto ret = Pattern::create();
    ret->setAnyChar();
    return ret;
}

Pattern_shared Pattern::Sequence(Pattern_shared_deque seq)
{
    auto ret = Pattern::create();
    ret->setSequence(seq);
    return ret;
}

Pattern_shared Pattern::OrSequence(Pattern_shared_deque seq)
{
    auto ret = Pattern::create();
    ret->setOrSequence(seq);
    return ret;
}

Pattern_shared Pattern::create()
{
    auto ret     = Pattern_shared(new Pattern());
    ret->own_ptr = ret;
    ret->setRepetitionFromType(PatternRepetitionType::Single);
    ret->greedy = false;
    return ret;
}

Pattern_shared create()
{
    return Pattern::create();
}

UString Result::repr_as_text()
{
    return repr_as_text({});
}

UString Result::repr_as_text(const Result_repr_as_text_opts &opts)
{
    // todo: use opts

    UString ret = std::format(
        R"+++(
wayround_i2p::ccutils::regexp::Result{{
        error?                    : {}
        original_sbject           : {}
        matched?                  : {}
        matched_repetitions_count : {}
        match_start               : {}
        match_end                 : {}
        corresponding_pattern     : {}
        submatches                : {}
}}
)+++",
        this->error ? this->error->Error() : "(no error)",
        opts.original_subject ? this->original_subject : "(disabled)",
        this->matched,
        this->matched_repetitions_count,
        this->match_start,
        this->match_end,
        opts.corresponding_pattern ? this->corresponding_pattern->repr_as_text() :
                                     "(disabled)",
        opts.submatches ? "(todo)" : "(disabled)"
    );
    // todo: does this return copy? use pointers?
    return ret;
}

Result_shared Result::create()
{
    auto ret     = Result_shared(new Result());
    ret->own_ptr = ret;
    return ret;
}

std::tuple<
    bool,
    size_t>
    isLineSplit(
        const UString &subject,
        std::size_t    start_at
    )
{
    auto pattern = Pattern::LineSplit();
    pattern->setRepetitionFromType(PatternRepetitionType::Single);

    auto res = match(pattern, subject, start_at);

    return std::tuple(res->matched, res->match_end - res->match_start);
}

const Result_shared match_single(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at
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
    ret->match_start           = start_at;

    const auto subject_length = subject.length();

    if (start_at > subject_length)
    {
        ret->error
            = wayround_i2p::ccutils::errors::New(
                "trying to work outside of subject"
            );

        return ret;
    }

    if (pattern->pattern_type == PatternType::Invalid)
    {
        ret->error
            = wayround_i2p::ccutils::errors::New(
                std::format(
                    "invalid `pattern_type` {}:{}",
                    __FILE__,
                    __LINE__
                )
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
                        " <- programming error : fixme: {} {}",
                        (unsigned char)pattern->pattern_type,
                        __FILE__,
                        __LINE__
                    )
                );

            return ret;
        }
        case PatternType::Invalid:
        {
            ret->error
                = wayround_i2p::ccutils::errors::New(
                    std::format(
                        "invalid `pattern_type` {}:{}",
                        __FILE__,
                        __LINE__
                    )
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
                            std::format(
                                "programming error - fixme: {} {}",
                                __FILE__,
                                __LINE__
                            )
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

            ret->matched = false;

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
                            std::format(
                                "programming error - fixme: {} {}",
                                __FILE__,
                                __LINE__
                            )
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
                        "there must be exactly 1 value inside of pattern->values"
                    );
                return ret;
            }

            auto target_char = pattern->values[0];

            if (start_at == subject_length)
            {
                ret->matched = false;
                return ret;
            }

            auto subj_char = subject[start_at];

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
                        "there must be exactly 2 values inside of pattern->values"
                    );
                return ret;
            }

            auto target_char0 = pattern->values[0];
            auto target_char1 = pattern->values[1];

            if (target_char0 > target_char1)
            {
                ret->error
                    = wayround_i2p::ccutils::errors::New(
                        "invalid char range specification"
                    );
                return ret;
            }

            if (start_at == subject_length)
            {
                ret->matched = false;
                return ret;
            }

            auto subj_char = subject[start_at];

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
            if (start_at >= subject_length)
            {
                ret->matched = false;
                return ret;
            }
            else
            {
                ret->matched     = true;
                ret->match_start = start_at;
                ret->match_end   = ret->match_start + 1;
                return ret;
            }
        }
        case PatternType::Sequence:
        {
            std::size_t end_tracker = start_at;

            for (auto x : pattern->subpatterns)
            {
                auto res = match(x, subject, end_tracker);

                if (res->error)
                {
                    ret->error = res->error;
                    return ret;
                }

                if (res->matched)
                {
                    end_tracker = res->match_end;
                }
                else
                {
                    ret->matched = false;
                    return ret;
                }
            }

            ret->matched   = true;
            ret->match_end = end_tracker;

            return ret;
        }
        case PatternType::OrSequence:
        {
            for (auto x : pattern->subpatterns)
            {
                auto res = match(x, subject, start_at);

                if (res->error)
                {
                    ret->error = res->error;
                    return ret;
                }

                if (res->matched)
                {
                    ret->matched   = true;
                    ret->match_end = res->match_end;
                    return ret;
                }
            }

            ret->matched = false;
            return ret;
        }
    }

    return ret;
}

const Result_shared match(
    const Pattern_shared pattern,
    const UString       &subject,
    std::size_t          start_at
)
{
    Result_shared ret           = nullptr;
    std::size_t   matched_count = 0;

    auto ex01 = std::experimental::scope_exit(
        [&ret, &matched_count]()
        {
            assert(ret);

            if (ret->error)
            {
                ret->matched = false;
            }

            if (!ret->matched)
            {
                ret->match_end = ret->match_start;
            }

            if (ret->matched)
            {
                ret->matched_repetitions_count = matched_count;
            }
        }
    );

    auto has_min = pattern->has_min;
    auto min     = pattern->min;
    auto has_max = pattern->has_max;
    auto max     = pattern->max;
    auto greedy  = pattern->greedy;

    if (!has_min)
    {
        min = 0;
    }

    if (!has_max)
    {
        max = 0;
    }

    if (has_max && has_min && min > max)
    {
        ret        = Result::create();
        ret->error = wayround_i2p::ccutils::errors::New(
            "invalid min/max in pattern"
        );
        return ret;
    }

    auto next_start = start_at;

    while (true)
    {
        auto res = match_single(pattern, subject, next_start);

        if (!ret)
        {
            ret = res;
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

            if (greedy && has_min && matched_count >= min)
            {
                return ret;
            }

            if (has_max && matched_count == max)
            {
                return ret;
            }
        }
        else
        {
            if (has_min && matched_count < min)
            {
                ret->matched = false;
                return ret;
            }
            else
            {
                ret->matched = true;
                return ret;
            }
        }
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
