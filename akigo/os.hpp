#ifndef WAYROUND_I2P_20241007_110017_740318
#define WAYROUND_I2P_20241007_110017_740318

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::os
{

using error_ptr   = wayround_i2p::akigo::builtin::error_ptr;
using size_type   = wayround_i2p::akigo::builtin::size_type;
using byte_vector = wayround_i2p::akigo::builtin::byte_vector;
using ustring     = wayround_i2p::akigo::builtin::ustring;

class File
{
};

using File_ptr = std::shared_ptr<File>;

error_ptr Remove(ustring name);

} // namespace wayround_i2p::akigo::os

#endif
