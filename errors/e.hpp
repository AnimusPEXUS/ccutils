#ifndef WAYROUND_I2P_20240511_072523_292537
#define WAYROUND_I2P_20240511_072523_292537

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::errors
{

using UString = wayround_i2p::ccutils::unicode::UString;

class error
{
  public:
    virtual UString Error() = 0;
};

using error_ptr = std::shared_ptr<error>;

class BasicStringError : public error
{
  public:
    BasicStringError(UString text);

    ~BasicStringError();

    UString Error();

  private:
    UString text;
};

error_ptr New(UString text);

} // namespace wayround_i2p::ccutils::errors

#endif
