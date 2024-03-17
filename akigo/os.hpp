#ifndef WAYROUND_I2P_20240310_155748_301370
#define WAYROUND_I2P_20240310_155748_301370

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::os
{

using error       = wayround_i2p::akigo::builtin::error;
using size_type   = wayround_i2p::akigo::builtin::size_type;
using byte_vector = wayround_i2p::akigo::builtin::byte_vector;

class File
{
};

using File_ptr = std::shared_ptr<File>;

} // namespace wayround_i2p::akigo::os

#endif
