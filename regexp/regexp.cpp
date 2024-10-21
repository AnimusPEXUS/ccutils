
#include <wayround_i2p/ccutils/regexp/regexp.hpp>

namespace wayround_i2p::ccutils::regexp
{

// todo: check functions ordering

void updateBonds(
    Pattern_shared_deque &v,
    Pattern_shared        set_parent
)
{
    for (auto &i : v)
    {
        i->parent = set_parent;
    }

    auto subpatterns_size = v.size();

    switch (subpatterns_size)
    {
        case 0:
            break;
        case 1:
        {
            auto &x = v[0];

            x->prev_sibling.reset();
            x->next_sibling = nullptr;

            break;
        }
        case 2:
        {
            auto &x1 = v[0];
            auto &x2 = v[subpatterns_size - 1];

            x1->prev_sibling.reset();
            x2->next_sibling = nullptr;

            x1->next_sibling = x2;
            x2->prev_sibling = x1;
            break;
        }
        default:
        {
            auto &xf = v[0];
            auto &xl = v[subpatterns_size - 1];

            xf->prev_sibling.reset();
            xl->next_sibling = nullptr;

            xf->next_sibling = v[1];
            xl->prev_sibling = v[subpatterns_size - 2];

            for (std::size_t i = 1; i < subpatterns_size - 1; i++)
            {
                auto &z         = v[i];
                z->prev_sibling = v[i - 1];
                z->next_sibling = v[i + 1];
            }
            break;
        }
    }
}

void Pattern::setSequenceParent(Pattern_shared val)
{
    auto x = findFirst();
    if (!x)
    {
        throw wayround_i2p::ccutils::errors::New(
            "couldn't get first element in Pattern sequence",
            __FILE__,
            __LINE__
        );
    }

    while (true)
    {
        x->parent = val;

        x = x->next_sibling;
        if (!x)
        {
            break;
        }
    }
}

Pattern_shared_deque &Pattern::makeSequenceDeque(Pattern_shared_deque &ret) const
{
    auto current = findFirst();
    if (!current)
    {
        throw wayround_i2p::ccutils::errors::New(
            "can't findFirst(",
            __FILE__,
            __LINE__
        );
    }
    ret.resize(0);
    while (true)
    {
        if (current)
        {
            ret.push_back(current);
            current = current->next_sibling;
        }
        else
        {
            break;
        }
    }
    return ret;
}

std::size_t Pattern::findSize() const
{
    auto x = findFirst();

    if (!x)
    {
        throw wayround_i2p::ccutils::errors::New(
            "fatal error: couldn't find first Pattern in sequence",
            __FILE__,
            __LINE__
        );
    }

    std::size_t ret = 1;

    while (true)
    {
        x = x->next_sibling;
        if (x)
        {
            ret++;
        }
        else
        {
            break;
        }
    }
    return ret;
}

Pattern_shared Pattern::findFirst() const
{
    if (prev_sibling.expired())
    {
        return own_ptr.lock();
    }

    auto cur = own_ptr.lock();
    while (true)
    {
        auto x = cur->prev_sibling.lock();
        if (!x)
        {
            return cur;
        }
        cur = x;
    }
}

Pattern_shared Pattern::findLast() const
{
    if (!next_sibling)
    {
        return own_ptr.lock();
    }

    auto cur = own_ptr.lock();
    while (true)
    {
        auto x = cur->next_sibling;
        if (!x)
        {
            return cur;
        }
        cur = x;
    }
}

Pattern_shared Pattern::setName(UString value)
{
    this->name = value;
    return Pattern_shared(this->own_ptr);
}

void Pattern::removeAllSubpatterns()
{
    notSubPattern = nullptr;
    orGroup.clear();
    group = nullptr;
}

bool Pattern::isCaseSensitive() const
{
    auto p = parent.lock();

    if (p && case_sensitive_from_parent)
    {
        return p->isCaseSensitive();
    }

    return case_sensitive;
}

UString Pattern::repr_as_text() const
{
    return repr_as_text(Pattern_repr_as_text_opts(false));
}

UString Pattern::repr_as_text(const Pattern_repr_as_text_opts &opts) const
{
    Pattern_shared_deque values;

    if (opts.siblings)
    {
        makeSequenceDeque(values);
    }
    else
    {
        values.push_back(own_ptr.lock());
    }

    UString sub_padding = std::format(
        "{}|           ",
        opts.padding
    );

    auto opts2    = opts;
    opts2.padding = std::format(
        "{}{}      ",
        sub_padding,
        "|"
    );

    UString ret = "";
    for (std::size_t i = 0; i < values.size(); i++)
    {
        const auto &ival = values[i];

        UString group_txt    = "";
        UString or_group_txt = "";

        if (opts.subpatterns)
        {
            // group_txt

            if (!ival->group)
            {
                group_txt = "(unset)";
            }
            else
            {
                std::size_t orGroup_size = ival->orGroup.size();

                group_txt += "\n";

                group_txt += ival->group->repr_as_text(opts2);
            }

            // or_group_txt

            std::size_t orGroup_size = ival->orGroup.size();

            if (orGroup_size == 0)
            {
                or_group_txt = "(empty)";
            }
            else
            {
                or_group_txt = std::format(
                    "(count: {})\n",
                    orGroup_size
                );

                for (std::size_t j = 0; j < orGroup_size; j++)
                {
                    or_group_txt += std::format(
                        "{}+-- #{} group item\n",
                        sub_padding,
                        j
                    );
                    or_group_txt += ival->orGroup[j]->repr_as_text(opts2);
                }
            }
        }

        ret += std::format(
            R"+++({0:}+---- sibling #{2:} wayround_i2p::ccutils::regexp::Pattern{{
{0:}|      name                      : {3:}
{0:}|      pattern_type              : {4:} ({5:})
{0:}|      values                    : {6:}
{0:}|      case_sensitive_from_parent: {7:}
{0:}|      case_sensitive            : {8:}
{0:}|      isCaseSensitive()         : {9:}
{0:}|      has_min                   : {10:}
{0:}|      has_max                   : {11:}
{0:}|      min                       : {12:}
{0:}|      max                       : {13:}
{0:}|      greedy                    : {14:}
{0:}|      notSubPattern             : {15:}
{0:}|      orGroup                   : {16:}
{0:}|      group                     : {17:}
{0:}+---- }}{1:})+++",
            opts.padding,
            "\n",
            (opts.siblings ? std::to_string(i) : "xx"),
            std::format("\"{}\"", ival->name),
            (int)(ival->pattern_type),
            wayround_i2p::ccutils::regexp::PatternTypeString(ival->pattern_type),
            "",
            ival->case_sensitive_from_parent,
            ival->case_sensitive,
            ival->isCaseSensitive(),
            ival->has_min,
            ival->has_max,
            ival->min,
            ival->max,
            ival->greedy,
            (!opts.subpatterns ? "(disabled)" : (!ival->notSubPattern ? "(unset)" : ival->notSubPattern->repr_as_text(opts))),
            (!opts.subpatterns ? "(disabled)" : or_group_txt),
            (!opts.subpatterns ? "(disabled)" : group_txt)
        );
    }

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

Pattern_shared Pattern::setNot(Pattern_shared notSubPattern)
{
    removeAllSubpatterns();
    this->pattern_type  = PatternType::Not;
    this->notSubPattern = notSubPattern;
    this->notSubPattern->setSequenceParent(own_ptr.lock());
    return own_ptr.lock();
}

Pattern_shared Pattern::setGroup(Pattern_shared group)
{
    removeAllSubpatterns();
    this->pattern_type = PatternType::Group;
    auto first         = group->findFirst();
    if (!first)
    {
        throw wayround_i2p::ccutils::errors::New(
            std::format(
                "couldn't find first element in sequence"
            ),
            __FILE__,
            __LINE__
        );
    }
    this->group = first;
    this->group->setSequenceParent(own_ptr.lock());
    return own_ptr.lock();
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
    Pattern_shared_deque x;
    for (const auto &i : val)
    {
        x.push_back(i);
    }

    return newOrGroup(x);
}

Pattern_shared Pattern::newGroup(std::initializer_list<Pattern_shared> val)
{
    auto ret = Pattern::create();
    ret->setGroup(val);
    return ret;
}

Pattern_shared Pattern::newGroup(Pattern_shared val)
{
    auto ret = Pattern::create();
    ret->setGroup(val);
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

std::size_t Result::findSize() const
{
    auto x = findFirst();

    if (!x)
    {
        throw wayround_i2p::ccutils::errors::New(
            "fatal error: couldn't find first Pattern in sequence",
            __FILE__,
            __LINE__
        );
    }

    std::size_t ret = 1;

    while (true)
    {
        x = x->next_sibling;
        if (x)
        {
            ret++;
        }
        else
        {
            break;
        }
    }
    return ret;
}

Result_shared Result::findFirst() const
{
    if (prev_sibling.expired())
    {
        return own_ptr.lock();
    }

    auto cur = own_ptr.lock();
    while (true)
    {
        auto x = cur->prev_sibling.lock();
        if (!x)
        {
            return cur;
        }
        cur = x;
    }
}

Result_shared Result::findLast() const
{
    if (!next_sibling)
    {
        return own_ptr.lock();
    }

    auto cur = own_ptr.lock();
    while (true)
    {
        auto x = cur->next_sibling;
        if (!x)
        {
            return cur;
        }
        cur = x;
    }
}

Result_shared Result::findByIndex(std::size_t index) const
{
    auto x = findFirst();
    if (!x)
    {
        throw wayround_i2p::ccutils::errors::New(
            "can't find first Result in sequence",
            __FILE__,
            __LINE__
        );
    }

    Result_shared ret;

    std::size_t i = 0;

    while (true)
    {
        if (i == index)
        {
            return x;
        }

        x = x->next_sibling;
        if (!x)
        {
            return nullptr;
        }
        i++;
    }
}

Result_shared Result::findByName(UString name, bool rec) const
{
    auto x = findFirst();
    if (!x)
    {
        throw wayround_i2p::ccutils::errors::New(
            "can't find first Result in sequence",
            __FILE__,
            __LINE__
        );
    }

    Result_shared ret;

    while (true)
    {
        if (x->corresponding_pattern->name == name)
        {
            return x;
        }

        if (rec)
        {
            if (subResult)
            {
                auto x2 = subResult->findByName(name, true);
                if (x2)
                {
                    return x2;
                }
            }
        }

        x = x->next_sibling;
        if (!x)
        {
            return nullptr;
        }
    }
}

Result_shared Result::findByNameRec(UString name) const
{
    return findByName(name, true);
}

Result_shared Result::operator[](UString name) const
{
    return findByName(name, false);
}

Result_shared Result::operator[](std::size_t index) const
{
    return findByIndex(index);
}

Result_shared_deque &Result::makeSequenceDeque(Result_shared_deque &ret) const
{
    auto current = findFirst();
    if (!current)
    {
        throw wayround_i2p::ccutils::errors::New(
            "can't findFirst(",
            __FILE__,
            __LINE__
        );
    }
    ret.resize(0);
    while (true)
    {
        if (current)
        {
            ret.push_back(current);
            current = current->next_sibling;
        }
        else
        {
            break;
        }
    }
    return ret;
}

Result_shared Result::getParent()
{
    return parent.lock();
}

Result_shared Result::findRoot()
{
    Result_shared ret = Result_shared(own_ptr);
    while (true)
    {
        auto x = ret->getParent();
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
    auto    ms = this->match_start;
    auto    l  = original_subject.length();
    ssize_t s  = this->match_end - this->match_start;

    if (ms > l || ms + s > l || s < 0)
    {
        throw wayround_i2p::ccutils::errors::New(
            std::format(
                "invalid parameters to .substr(): "
                "start = {}, len = {}. str len = {}",
                this->match_start,
                s,
                l
            ),
            __FILE__,
            __LINE__
        );
    }

    return original_subject.substr(ms, s);
}

UString Result::repr_as_text() const
{
    return repr_as_text(Result_repr_as_text_opts(false));
}

UString Result::repr_as_text(const Result_repr_as_text_opts &opts) const
{
    Result_shared_deque values;

    makeSequenceDeque(values);

    UString sub_padding = std::format(
        "{}|           ",
        opts.padding
    );

    UString ret;
    for (std::size_t i = 0; i < values.size(); i++)
    {
        const auto &ival = values[i];

        UString subm = "";

        if (opts.subResults)
        {
            if (!ival->subResult)
            {
                subm = "(none)";
            }
            else
            {

                std::size_t subResults_size = ival->subResult->findSize();

                subm = std::format(
                    "(count: {})\n",
                    subResults_size
                );

                std::size_t   c = 0;
                Result_shared i = ival->subResult;

                while (i)
                {
                    auto cpn  = i->corresponding_pattern->name;
                    subm     += std::format(
                        "{}+-- #{} submatch: {}\n",
                        sub_padding,
                        c,
                        (cpn != "" ?
                                 std::format("\"{}\"", cpn) :
                                 "(unnamed)")
                    );
                    auto opts2    = opts;
                    opts2.padding = std::format(
                        "{}{}      ",
                        sub_padding,
                        (c < subResults_size - 1 ? "|" : " ")
                    );
                    subm += i->repr_as_text(opts2);

                    i = i->next_sibling;
                    c++;
                }
            }
        }

        UString corp_patt;

        if (opts.corresponding_pattern)
        {
            if (!ival->corresponding_pattern)
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

                auto opts2        = Pattern_repr_as_text_opts(true);
                opts2.subpatterns = opts.corresponding_pattern_subpatterns;
                opts2.siblings    = false;
                opts2.padding     = std::format(
                    "{}|      ",
                    padding
                );
                corp_patt += ival->corresponding_pattern->repr_as_text(opts2);
            }
        }

        ret += std::format(
            R"+++({0:}+---- sibling #{2:} wayround_i2p::ccutils::regexp::Result{{
{0:}|      error?                    : {3:}
{0:}|      original_subject          : {4:}
{0:}|      matched?                  : {5:}
{0:}|      matched_repetitions_count : {6:}
{0:}|      match_start               : {7:}
{0:}|      match_end                 : {8:}
{0:}|      matched string            : {9:}
{0:}|      corresponding_pattern     : {10:}
{0:}|      subResults                : {11:}
{0:}+---- }}{1:})+++",
            opts.padding,
            "\n",
            i,
            ival->error ? ival->error->Error() : "(no error)",
            opts.original_subject ? ival->original_subject : "(disabled)",
            ival->matched,
            ival->matched_repetitions_count,
            ival->match_start,
            ival->match_end,
            (ival->matched ? std::format("\"{}\"", ival->getMatchedString()) : "(dismatched)"),
            (opts.corresponding_pattern ? corp_patt : "(disabled)"),
            (opts.subResults ? subm : "(disabled)")
        );
    }

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
    ret->parent                = parent_result;
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
            auto res = match(
                pattern->notSubPattern,
                subject,
                start_at,
                ret
            );

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

            ret->matched   = false;
            ret->subResult = res;
            return ret;
        }

        case PatternType::OrGroup:
        {
            for (auto &x : pattern->orGroup)
            {
                auto res = match(
                    x,
                    subject,
                    start_at,
                    ret
                );

                if (res->error)
                {
                    ret->error = res->error;
                    return ret;
                }

                if (res->matched)
                {
                    ret->matched   = true;
                    ret->match_end = res->match_end;
                    ret->subResult = res;
                    return ret;
                }
            }

            ret->matched = false;
            return ret;
        }

        case PatternType::Group:
        {
            auto res = match(
                pattern->group,
                subject,
                start_at,
                ret
            );

            ret->subResult = res;

            if (res->error)
            {
                ret->error = res->error;
                return ret;
            }

            if (res->matched)
            {
                ret->matched = true;
                auto last    = res->findLast();
                if (!last)
                {
                    throw wayround_i2p::ccutils::errors::New(
                        "Can't find last Result in sequence",
                        __FILE__,
                        __LINE__
                    );
                }
                ret->match_end = last->match_end;
            }
            else
            {
                ret->matched = false;
            }
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
        ret         = Result::create();
        ret->parent = parent_result;
        ret->error  = wayround_i2p::ccutils::errors::New(
            "invalid min/max in pattern",
            __FILE__,
            __LINE__
        );
        return ret;
    }

    Result_shared first_matched_next;
    Result_shared last_matched_next;

    auto next_start = start_at;

    bool continue_after_try_next = false;

    while (true)
    {
        auto next_pattern = pattern->next_sibling;

        auto res = match_single(
            pattern,
            subject,
            next_start,
            parent_result
        );

        if (!ret)
        {
            ret = res;

            if (parent_result && ret->parent.expired())
            {
                throw wayround_i2p::ccutils::errors::New(
                    "parent_result && !ret->parent_result",
                    __FILE__,
                    __LINE__
                );
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

            if (has_max && matched_count == max)
            {
                if (next_pattern)
                {
                    goto try_next_and_summarize_next;
                }
                else
                {
                    goto summarize_main;
                }
            }

            if (has_min && matched_count < min)
            {
                goto just_continue;
            }

            if (next_pattern)
            {
                goto try_next_and_continue;
            }
            else
            {
                goto just_continue;
            }
        }
        else
        {
            if (has_min && min != 0 && matched_count < min)
            {
                if (next_pattern)
                {
                    goto summarize_next;
                }
                else
                {
                    goto summarize_main;
                }
            }
            else
            {
                if (next_pattern)
                {
                    goto try_next_and_summarize_next;
                }
                else
                {
                    goto summarize_main;
                }
            }
        }

    just_continue:
    {
        continue;
    }

    try_next_and_continue:
    {
        continue_after_try_next = true;
        goto try_next;
    }

    try_next_and_summarize_next:
    {
        continue_after_try_next = false;
        goto try_next;
    }

    try_next:
    {
        auto next_sibling_result = match(
            next_pattern,
            subject,
            next_start,
            nullptr
        );

        if (!next_sibling_result)
        {
            ret->error = wayround_i2p::ccutils::errors::New(
                "match() on next sibling returned null",
                __FILE__,
                __LINE__
            );
            return ret;
        }

        if (next_sibling_result->error)
        {
            // todo: is this correct error passing/returning?
            ret->next_sibling                 = next_sibling_result;
            next_sibling_result->prev_sibling = ret;
            ret->error                        = next_sibling_result->error;
            return ret;
        }

        if (next_sibling_result->matched)
        {
            if (!first_matched_next)
            {
                first_matched_next = next_sibling_result;
            }
            last_matched_next = next_sibling_result;

            if (continue_after_try_next)
            {
                continue;
            }
            else
            {
                goto summarize_next;
            }
        }
        else
        {
            goto summarize_next;
        }
    }

    summarize_next:
    {
        if (!first_matched_next && !last_matched_next)
        {
            ret->matched = false;
        }
        else
        {
            Result_shared x;
            if (greedy)
            {
                x = (first_matched_next ? first_matched_next : last_matched_next);
            }
            else
            {
                x = (last_matched_next ? last_matched_next : first_matched_next);
            }
            ret->match_end    = x->match_start;
            ret->next_sibling = x;
            x->prev_sibling   = ret;
        }
    }

    summarize_main:
    {
        break;
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

Pattern_shared makeSequence(std::initializer_list<Pattern_shared> val)
{
    if (val.size() == 0)
    {
        throw wayround_i2p::ccutils::errors::New(
            "`val` MUST BE not empty",
            __FILE__,
            __LINE__
        );
    }

    Pattern_shared_deque x;
    for (const auto &i : val)
    {
        x.push_back(i);
    }

    return makeSequence(x);
}

UString PatternTypeString(PatternType v)
{
    switch (v)
    {
        default:
            throw wayround_i2p::ccutils::errors::New(
                "unsupported value",
                __FILE__,
                __LINE__
            );
        case PatternType::Invalid:
            return "Invalid";

        case PatternType::TextStart:
            return "TextStart";
        case PatternType::TextEnd:
            return "TextEnd";
        case PatternType::LineStart:
            return "LineStart";
        case PatternType::LineEnd:
            return "LineEnd";

        case PatternType::LineSplit:
            return "LineSplit";

        case PatternType::ExactChar:
            return "ExactChar";
        case PatternType::CharRange:
            return "CharRange";

        case PatternType::AnyChar:
            return "AnyChar";

        case PatternType::CharIsAlpha:
            return "CharIsAlpha";
        case PatternType::CharIsLower:
            return "CharIsLower";
        case PatternType::CharIsUpper:
            return "CharIsUpper";
        case PatternType::CharIsPunct:
            return "CharIsPunct";
        case PatternType::CharIsDigit:
            return "CharIsDigit";
        case PatternType::CharIsXDigit:
            return "CharIsXDigit";
        case PatternType::CharIsSpace:
            return "CharIsSpace";
        case PatternType::CharIsBlank:
            return "CharIsBlank";

        case PatternType::Not:
            return "Not";

        case PatternType::OrGroup:
            return "OrGroup";
        case PatternType::Group:
            return "Group";
    }
}

} // namespace wayround_i2p::ccutils::regexp
