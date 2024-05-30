
#include <wayround_i2p/ccutils/regexp/regexp.hpp>

namespace wayround_i2p::ccutils::regexp
{

Pattern_shared Pattern::create()
{
    auto ret     = Pattern_shared(new Pattern());
    ret->own_ptr = ret;
    return ret;
}

Result_shared Result::create()
{
    auto ret     = Result_shared(new Result());
    ret->own_ptr = ret;
    return ret;
}

const Result_shared match(
    const Pattern_shared pattern,
    const UString        subject,
    const std::size_t    start_at
)
{
    // todo: separate pattern sanity checks to inprove performance?

    auto ret = Result::create();

    ret->original_subject      = subject;
    ret->corresponding_pattern = pattern;
    ret->match_start           = start_at;

    if (pattern->pattern_type == PatternType::Invalid)
    {
        ret->matched = false;
        // todo: report error
        return ret;
    }

    switch (pattern->pattern_type)
    {
        default:
        {
            ret->matched = false;
            // todo: report error
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
            auto subj_length = subject.length();
            ret->match_end   = start_at;
            ret->matched     = start_at == subj_length;
            return ret;
        }
        case PatternType::LineStart:
        case PatternType::LineEnd:
        {
            ret->matched = false;
            // todo: implement
            // todo: report error
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
                    ret->matched = false;
                    // todo: report error
                    return ret;
                }
                case 0:
                {
                    ret->matched = false;
                    // todo: not an error imo. simply missmatch
                    return ret;
                }
                case 1:
                {
                    if (substr == UString(std::vector<UChar>{0x1a}))
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
                    if (substr == std::vector<UChar>{0x0d, 0x0a})
                    {
                        ret->matched   = true;
                        ret->match_end = ret->match_start + 2;
                        return ret;
                    }
                    else
                    {
                        ret->matched = false;
                        return ret;
                    }
                }
            }

            return ret;
        }
    }

    return ret;
}

} // namespace wayround_i2p::ccutils::regexp
