#ifndef WAYROUND_I2P_20240408_004458_736737
#define WAYROUND_I2P_20240408_004458_736737

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
    virtual UString Error() = 0;
};

using error_ptr = std::shared_ptr<error>;

class BasicStringError : public error
{
  public:
    BasicStringError(UString text) :
        text(text)
    {
    }

    ~BasicStringError()
    {
    }

    UString Error()
    {
        return text;
    }

  private:
    UString text;
};

error_ptr New(UString text)
{
    return error_ptr(dynamic_cast<error *>(new BasicStringError(text)));
}

} // namespace wayround_i2p::ccutils::errors

#endif
