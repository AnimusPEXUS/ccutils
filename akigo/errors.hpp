#ifndef WAYROUND_I2P_20240325_015305_233436
#define WAYROUND_I2P_20240325_015305_233436

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::errors
{
using error_ptr = wayround_i2p::akigo::builtin::error_ptr;
using error     = wayround_i2p::akigo::builtin::error;
using ustring   = wayround_i2p::akigo::builtin::ustring;

class BasicStringError : public error
{
  public:
    BasicStringError(ustring text) :
        text(text)
    {
    }

    ~BasicStringError()
    {
    }

    ustring Error()
    {
        return text;
    }

  private:
    ustring text;
};

error_ptr New(ustring text)
{
    auto ret = error_ptr(dynamic_cast<error *>(new BasicStringError(text)));
    return ret;
}

} // namespace wayround_i2p::akigo::errors

#endif
