#ifndef WAYROUND_I2P_20250205_113427_119348
#define WAYROUND_I2P_20250205_113427_119348

#include <wayround_i2p/ccutils/errors/e.hpp>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::errors
{
using error_ptr = wayround_i2p::akigo::builtin::error_ptr;
using error     = wayround_i2p::akigo::builtin::error;
using ustring   = wayround_i2p::akigo::builtin::ustring;

inline error_ptr New(ustring text, ustring file, std::size_t line)
{
    return wayround_i2p::ccutils::errors::New(
        text,
        file,
        line
    );
}

} // namespace wayround_i2p::akigo::errors

#endif
