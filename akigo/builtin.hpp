#ifndef WAYROUND_I2P_20250205_113427_118165
#define WAYROUND_I2P_20250205_113427_118165

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

// using error     = wayround_i2p::ccutils::errors::error;
using error_ptr = wayround_i2p::ccutils::errors::error_ptr;

using go_byte = std::uint8_t;
using go_rune = std::int32_t;

using go_int    = int;
using go_uint   = unsigned int;
using go_int64  = std::int64_t;
using go_uint64 = std::uint64_t;

using size_type   = std::vector<std::uint8_t>::size_type;
using byte_vector = std::vector<std::uint8_t>;
using byte_slice  = byte_vector;
using ustring     = wayround_i2p::ccutils::unicode::UString;

} // namespace wayround_i2p::akigo::builtin

#define CCUTILS_MACRO_USE_TYPES_SET00                            \
    using error_ptr  = wayround_i2p::akigo::builtin::error_ptr;  \
    using size_type  = wayround_i2p::akigo::builtin::size_type;  \
    using byte_slice = wayround_i2p::akigo::builtin::byte_slice; \
    using ustring    = wayround_i2p::akigo::builtin::ustring;    \
    using go_byte    = wayround_i2p::akigo::builtin::go_byte;    \
    using go_rune    = wayround_i2p::akigo::builtin::go_rune;    \
    using go_int     = wayround_i2p::akigo::builtin::go_int;     \
    using go_uint    = wayround_i2p::akigo::builtin::go_uint;    \
    using go_int64   = wayround_i2p::akigo::builtin::go_int64;   \
    using go_uint64  = wayround_i2p::akigo::builtin::go_uint64;

#endif
