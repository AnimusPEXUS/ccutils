#ifndef WAYROUND_I2P_20240308_140051_32431
#define WAYROUND_I2P_20240308_140051_32431

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// note: no 'ccutils' in namespace.
//       why? - because I fill akigo as separate project but don't
//       think it should really be separate from ccutils.

namespace wayround_i2p::akigo::builtin
{
using size_type   = std::vector<std::uint8_t>::size_type;
using byte_vector = std::vector<std::uint8_t>;

class error_nsp
{
    virtual std::string Error() = 0;
};

using error = std::shared_ptr<error_nsp>;

} // namespace wayround_i2p::akigo::builtin

#endif
