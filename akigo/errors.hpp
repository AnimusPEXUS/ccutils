#ifndef WAYROUND_I2P_20260131_182305_0000231023
#define WAYROUND_I2P_20260131_182305_0000231023

#include <wayround_i2p/ccutils/errors/e.hpp>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::errors
{

using error_ptr = wayround_i2p::akigo::builtin::error_ptr;
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
