
#include <wayround_i2p/ccutils/regexp/regexp.hpp>

namespace wayround_i2p::ccutils::regexp
{

Pattern_shared Pattern::create(PatternType type)
{
    auto ret          = Pattern_shared(new Pattern());
    ret->own_ptr      = ret;
    ret->pattern_type = type;
    ret->setRepetitionFromType(PatternRepetitionType::Single);
    ret->greedy = false;
    return ret;
}

error_ptr Pattern::setRepetitionFromType(
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
            return wayround_i2p::ccutils::errors::New(
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
            return nullptr;

        case PatternRepetitionType::NoneOrOne:
            this->has_min = true;
            this->has_max = true;
            // this->min     = 0;
            this->max     = 1;
            return nullptr;

        case PatternRepetitionType::NoneOrMore:
            this->has_min = true;
            // this->has_max = false;
            // this->min     = 0;
            // this->max     = 0;
            return nullptr;

        case PatternRepetitionType::OneOrMore:
            this->has_min = true;
            // this->has_max = false;
            this->min     = 1;
            // this->max     = 0;
            return nullptr;
    }

    return wayround_i2p::ccutils::errors::New(
        std::format(
            "unexpected error. {}:{}",
            __FILE__,
            __LINE__
        )
    );
}

void Pattern::setMinCount(std::size_t val)
{
    this->has_min = true;
    this->min     = val;
}

void Pattern::setMaxCount(std::size_t val)
{
    this->has_max = true;
    this->max     = val;
}

void Pattern::setMinMaxCount(std::size_t min, std::size_t max)
{
    setMinCount(min);
    setMaxCount(max);
}

void Pattern::setExactCount(std::size_t val)
{
    setMinCount(val);
    setMaxCount(val);
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
    auto pattern = Pattern::create(PatternType::LineSplit);
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
    const Result_shared_sequence,
    wayround_i2p::ccutils::errors::error_ptr>
    findAll(
        const Pattern_shared pattern,
        const UString       &subject,
        std::size_t          start_at
    )
{
    // todo: add overlap prevention option?

    Result_shared_sequence                   ret;
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
            return std::tuple(Result_shared_sequence{}, ret_err);
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
