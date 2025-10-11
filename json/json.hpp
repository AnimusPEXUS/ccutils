#ifndef WAYROUND_I2P_20250205_113427_163044
#define WAYROUND_I2P_20250205_113427_163044

#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/regexp/regexp.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::json
{

namespace regexp = wayround_i2p::ccutils::regexp;

using error_ptr = wayround_i2p::ccutils::errors::error_ptr;
using UString   = wayround_i2p::ccutils::unicode::UString;
using UChar     = wayround_i2p::ccutils::unicode::UChar;

/*
regexp::Pattern_shared json_PATTERN();
regexp::Pattern_shared value_PATTERN();
regexp::Pattern_shared object_PATTERN();
regexp::Pattern_shared members_PATTERN();
regexp::Pattern_shared member_PATTERN();
regexp::Pattern_shared array_PATTERN();
regexp::Pattern_shared elements_PATTERN();
regexp::Pattern_shared element_PATTERN();
regexp::Pattern_shared string_PATTERN();
regexp::Pattern_shared characters_PATTERN();
regexp::Pattern_shared character_PATTERN();
regexp::Pattern_shared escape_PATTERN();
regexp::Pattern_shared hex_PATTERN();
regexp::Pattern_shared number_PATTERN();
regexp::Pattern_shared integer_PATTERN();
regexp::Pattern_shared digits_PATTERN();
regexp::Pattern_shared digit_PATTERN();
regexp::Pattern_shared onenine_PATTERN();
regexp::Pattern_shared fraction_PATTERN();
regexp::Pattern_shared exponent_PATTERN();
regexp::Pattern_shared sign_PATTERN();
regexp::Pattern_shared ws_PATTERN();
*/

const regexp::Pattern_shared value_PATTERN
    = regexp::Pattern::newOrGroup(
        {object_PATTERN,
         array_PATTERN,
         string_PATTERN,
         number_PATTERN,
         regexp::make_string_matching_pattern("true"),
         regexp::make_string_matching_pattern("false"),
         regexp::make_string_matching_pattern("null")
        }
    );

const regexp::Pattern_shared json_PATTERN
    = element_PATTERN;

const regexp::Pattern_shared object_PATTERN
    = regexp::Pattern::newOrGroup(
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

const regexp::Pattern_shared members_PATTERN
    = return regexp::Pattern::newOrGroup(
        {member_PATTERN(),
         regexp::Pattern::newGroup(
             {member_PATTERN(),
              regexp::Pattern::newExactChar(","),
              members_PATTERN()
             }
         )
        }
    );

const regexp::Pattern_shared member_PATTERN
    = regexp::Pattern::newGroup(
        {ws_PATTERN(),
         regexp::Pattern::newExactChar(":"),
         ws_PATTERN(),
         element_PATTERN()
        }
    );

const regexp::Pattern_shared array_PATTERN
    = regexp::Pattern::newOrGroup(
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

const regexp::Pattern_shared elements_PATTERN
    = regexp::Pattern::newOrGroup(
        {element_PATTERN(),
         regexp::Pattern::newGroup(
             {element_PATTERN(),
              regexp::Pattern::newExactChar(","),
              elements_PATTERN()
             }
         )
        }
    );

const regexp::Pattern_shared element_PATTERN
    = regexp::Pattern::newGroup(
        {ws_PATTERN(),
         value_PATTERN(),
         ws_PATTERN()
        }
    );

const regexp::Pattern_shared string_PATTERN
    = regexp::Pattern::newGroup(
        {regexp::Pattern::newExactChar("\""),
         characters_PATTERN(),
         regexp::Pattern::newExactChar("\"")
        }
    );

const regexp::Pattern_shared characters_PATTERN
    = character_PATTERN()->setMinCount(0)->unsetMaxCount();

const regexp::Pattern_shared character_PATTERN
    = regexp::Pattern::newOrGroup(
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

const regexp::Pattern_shared escape_PATTERN
    = regexp::Pattern::newOrGroup(
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

const regexp::Pattern_shared hex_PATTERN
    = regexp::Pattern::newCharIsXDigit();

const regexp::Pattern_shared number_PATTERN
    = regexp::Pattern::newGroup(
        {integer_PATTERN(),
         fraction_PATTERN(),
         exponent_PATTERN()
        }
    );

const regexp::Pattern_shared integer_PATTERN
    = regexp::Pattern::newOrGroup(
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

const regexp::Pattern_shared digits_PATTERN
    = regexp::Pattern::newGroup(
        {digit_PATTERN(),
         digits_PATTERN()}
    );

const regexp::Pattern_shared digit_PATTERN
    = regexp::Pattern::newOrGroup(
        {regexp::Pattern::newExactChar("0"),
         onenine_PATTERN()}
    );

const regexp::Pattern_shared onenine_PATTERN
    = regexp::Pattern::newCharRange("1", "9", true);

const regexp::Pattern_shared fraction_PATTERN
    = regexp::Pattern::newGroup(
          {regexp::Pattern::newExactChar("."),
           digits_PATTERN()}
    )
          ->setMinMaxCount(0, 1);

const regexp::Pattern_shared exponent_PATTERN
    = regexp::Pattern::newGroup(
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

const regexp::Pattern_shared sign_PATTERN
    = regexp::Pattern::newOrGroup(
          {regexp::Pattern::newExactChar("-"),
           regexp::Pattern::newExactChar("+")
          }
    )
          ->setMinMaxCount(0, 1);

const regexp::Pattern_shared ws_PATTERN
    = regexp::Pattern::newOrGroup(
          {regexp::Pattern::newExactChar(0x20),
           regexp::Pattern::newExactChar(0x0a),
           regexp::Pattern::newExactChar(0x0d),
           regexp::Pattern::newExactChar(0x09)
          }
    )
          ->setMinMaxCount(0, 1);

class JSONObject;
class JSONArray;
class JSONElement;
class JSON;

using JSONObject_shared  = std::shared_ptr<JSONObject>;
using JSONArray_shared   = std::shared_ptr<JSONArray>;
using JSONElement_shared = std::shared_ptr<JSONElement>;
using JSON_shared        = std::shared_ptr<JSON>;

enum class JSONType : unsigned char
{
    invalid = 0,
    object,
    array,
    string,
    number,
    t,
    f,
    n
};

class JSONObject
{
  public:
    static JSONObject_shared create();

  protected:
    JSONObject();

  public:
    ~JSONObject();

    std::map<UString, JSONElement_shared> pairs;
};

class JSONArray
{
  public:
    static JSONArray_shared create();

  protected:
    JSONArray();

  public:
    ~JSONArray();

    std::deque<JSONElement_shared> items;
};

class JSONNumber
{
  public:
};

class JSONElement
{
  public:
    static JSONElement_shared create();

  protected:
    JSONElement();

  public:
    ~JSONElement();

    JSONType type;
    union
    {
        JSONObject_shared obj;
        JSONArray_shared  arr;
        UString           str;
        JSONNumber        num;
    };
};

class JSON
{
  public:
    static JSON_shared create();

  protected:
    JSON();

  public:
    ~JSON();

  public:
    JSONElement_shared element;
};

} // namespace wayround_i2p::ccutils::json

#endif
