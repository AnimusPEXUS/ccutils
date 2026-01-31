#ifndef WAYROUND_I2P_20260131_190131_5823930000
#define WAYROUND_I2P_20260131_190131_5823930000

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::os
{

CCUTILS_MACRO_USE_TYPES_SET00

class File
{
};

using File_ptr = std::shared_ptr<File>;

error_ptr Remove(ustring name);

} // namespace wayround_i2p::akigo::os

#endif
