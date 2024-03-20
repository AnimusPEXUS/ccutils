#ifndef WAYROUND_I2P_20240310_155748_30004a
#define WAYROUND_I2P_20240310_155748_30004a

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::errors
{
using error_ptr = wayround_i2p::akigo::builtin::error_ptr;

class error
{
  public:
    static error_ptr New(std::string text)
    {
        auto ret = error_ptr(new error(text));
        return ret;
    }

  protected:
    error(std::string text) :
        text(text)
    {
    }

    ~error()
    {
    }

  private:
    std::string text;
};

} // namespace wayround_i2p::akigo::errors

#endif
