
#include "json.hpp"

#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::json
{

/*
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
    return character_PATTERN()->setMinCount(0)->unsetMaxCount();
}

regexp::Pattern_shared character_PATTERN()
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

regexp::Pattern_shared escape_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {regexp::Pattern::newExactChar("\""),
         regexp::Pattern::newExactChar("\\"),
         regexp::Pattern::newExactChar("/"),
         regexp::Pattern::newExactChar("b"),
         regexp::Pattern::newExactChar("f"),
         regexp::Pattern::newExactChar("n"),
         regexp::Pattern::newExactChar("r"),
         regexp::Pattern::newExactChar("t"),
         regexp::Pattern::newOrGroup(
             {regexp::Pattern::newExactChar("u"),
              hex_PATTERN()->setMinMaxCount(1, 4)}
         )
        }
    );
}

regexp::Pattern_shared hex_PATTERN()
{
    return regexp::Pattern::newCharIsXDigit();
}

regexp::Pattern_shared number_PATTERN()
{
    return regexp::Pattern::newGroup(
        {integer_PATTERN(),
         fraction_PATTERN(),
         exponent_PATTERN()
        }
    );
}

regexp::Pattern_shared integer_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {digits_PATTERN(),

         regexp::Pattern::newGroup(
             {onenine_PATTERN(),
              digits_PATTERN()}
         ),

         regexp::Pattern::newGroup(
             {regexp::Pattern::newExactChar("-"),
              digits_PATTERN()}
         ),

         regexp::Pattern::newGroup(
             {regexp::Pattern::newExactChar("-"),
              onenine_PATTERN(),
              digits_PATTERN()}
         )
        }
    );
}

regexp::Pattern_shared digits_PATTERN()
{
    return regexp::Pattern::newGroup(
        {digit_PATTERN(),
         digits_PATTERN()}
    );
}

regexp::Pattern_shared digit_PATTERN()
{
    return regexp::Pattern::newOrGroup(
        {regexp::Pattern::newExactChar("0"),
         onenine_PATTERN()}
    );
}

regexp::Pattern_shared onenine_PATTERN()
{
    return regexp::Pattern::newCharRange("1", "9", true);
}

regexp::Pattern_shared fraction_PATTERN()
{
    return regexp::Pattern::newGroup(
               {regexp::Pattern::newExactChar("."),
                digits_PATTERN()}
    )
        ->setMinMaxCount(0, 1);
}

regexp::Pattern_shared exponent_PATTERN()
{
    return regexp::Pattern::newGroup(
               {regexp::Pattern::newOrGroup(
                    {regexp::Pattern::newExactChar("e"),
                     regexp::Pattern::newExactChar("E")
                    }
                ),
                sign_PATTERN(),
                digits_PATTERN()
               }
    )
        ->setMinMaxCount(0, 1);
}

regexp::Pattern_shared sign_PATTERN()
{
    return regexp::Pattern::newOrGroup(
               {regexp::Pattern::newExactChar("-"),
                regexp::Pattern::newExactChar("+")
               }
    )
        ->setMinMaxCount(0, 1);
}

regexp::Pattern_shared ws_PATTERN()
{
    return regexp::Pattern::newOrGroup(
               {regexp::Pattern::newExactChar(0x20),
                regexp::Pattern::newExactChar(0x0a),
                regexp::Pattern::newExactChar(0x0d),
                regexp::Pattern::newExactChar(0x09)
               }
    )
        ->setMinMaxCount(0, 1);
}
*/

} // namespace wayround_i2p::ccutils::json
