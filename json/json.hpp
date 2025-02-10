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
