
#include "json.hpp"

#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::json
{

regexp::Pattern_shared json_PATTERN()
{
    return element_PATTERN();
}

regexp::Pattern_shared value_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {object_PATTERN(),
         array_PATTERN(),
         string_PATTERN(),
         number_PATTERN(),
         regexp::make_string_matching_pattern("true"),
         regexp::make_string_matching_pattern("false"),
         regexp::make_string_matching_pattern("null")
        }
    );
}

regexp::Pattern_shared object_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {
            regexp::Pattern::newGroup(
                {regexp::Pattern::newExactChar("{"),
                 ws_PATTERN(),
                 regexp::Pattern::newExactChar("}")}
            ),
            regexp::Pattern::newGroup(
                {regexp::Pattern::newExactChar("{"),
                 members_PATTERN(),
                 regexp::Pattern::newExactChar("}")}
            ),
        }
    );
}

regexp::Pattern_shared members_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {member_PATTERN(),
         regexp::Pattern::newGroup(
             {member_PATTERN(),
              regexp::Pattern::newExactChar(","),
              members_PATTERN()
             }
         )
        }
    );
}

regexp::Pattern_shared member_PATTERN()
{
    return regexp::Pattern::newGroup(
        {ws_PATTERN(),
         regexp::Pattern::newExactChar(":"),
         ws_PATTERN(),
         element_PATTERN()
        }
    );
}

regexp::Pattern_shared array_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {
            regexp::Pattern::newGroup(
                {regexp::Pattern::newExactChar("["),
                 ws_PATTERN(),
                 regexp::Pattern::newExactChar("]")}
            ),
            regexp::Pattern::newGroup(
                {regexp::Pattern::newExactChar("["),
                 elements_PATTERN(),
                 regexp::Pattern::newExactChar("]")}
            ),
        }
    );
}

regexp::Pattern_shared elements_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {element_PATTERN(),
         regexp::Pattern::newGroup(
             {element_PATTERN(),
              regexp::Pattern::newExactChar(","),
              elements_PATTERN()
             }
         )
        }
    );
}

regexp::Pattern_shared element_PATTERN()
{
    return regexp::Pattern::newGroup(
        {ws_PATTERN(),
         value_PATTERN(),
         ws_PATTERN()
        }
    );
}

regexp::Pattern_shared string_PATTERN()
{
    return regexp::Pattern::newGroup(
        {regexp::Pattern::newExactChar("\""),
         characters_PATTERN(),
         regexp::Pattern::newExactChar("\"")
        }
    );
}

regexp::Pattern_shared characters_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {regexp::Pattern::newIsCharValidFunc(
             [](
                 UChar                        x,
                 const regexp::Pattern_shared pat,
                 std::size_t                  start_at
             ) -> bool
             {
                 auto xx = x.as_int32();
                 return xx >= 0x20 && xx <= 0x10ffff;
             }
         ),

         regexp::Pattern::newOrGroup(
             {regexp::Pattern::newGroup(
                 {regexp::Pattern::newExactChar("\\"),
                  escape_PATTERN()
                 }
             )
             }
         )
        }
    );
}

regexp::Pattern_shared character_PATTERN()
{
}

regexp::Pattern_shared escape_PATTERN()
{
}

regexp::Pattern_shared hex_PATTERN()
{
}

regexp::Pattern_shared number_PATTERN()
{
}

regexp::Pattern_shared integer_PATTERN()
{
}

regexp::Pattern_shared digits_PATTERN()
{
}

regexp::Pattern_shared digit_PATTERN()
{
}

regexp::Pattern_shared onenine_PATTERN()
{
}

regexp::Pattern_shared fraction_PATTERN()
{
}

regexp::Pattern_shared exponent_PATTERN()
{
}

regexp::Pattern_shared sign_PATTERN()
{
}

regexp::Pattern_shared ws_PATTERN()
{
}

} // namespace wayround_i2p::ccutils::json
