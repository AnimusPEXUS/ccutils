#ifndef WAYROUND_I2P_20240711_143336_984435
#define WAYROUND_I2P_20240711_143336_984435

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <wayround_i2p/ccutils/errors/e.hpp>
#include <wayround_i2p/ccutils/unicode/u.hpp>

// note: no 'ccutils' in namespace.
//       why? - because I feel akigo as separate project but don't
//       think it should really be separate from ccutils.

namespace wayround_i2p::akigo::builtin
{

using error     = wayround_i2p::ccutils::errors::error;
using error_ptr = wayround_i2p::ccutils::errors::error_ptr;

using size_type   = std::vector<std::uint8_t>::size_type;
using byte_vector = std::vector<std::uint8_t>;
using ustring     = wayround_i2p::ccutils::unicode::UString;

} // namespace wayround_i2p::akigo::builtin

#endif
