#ifndef WAYROUND_I2P_20240601_083133_760568
#define WAYROUND_I2P_20240601_083133_760568

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::io
{

using error_ptr   = wayround_i2p::akigo::builtin::error_ptr;
using size_type   = wayround_i2p::akigo::builtin::size_type;
using byte_vector = wayround_i2p::akigo::builtin::byte_vector;

class Reader
{
    virtual std::tuple<size_type, error_ptr> Read(byte_vector p) = 0;
};

using Reader_ptr = std::shared_ptr<Reader>;

class Writer
{
    virtual std::tuple<size_type, error_ptr> Write(byte_vector p) = 0;
};

using Writer_ptr = std::shared_ptr<Writer>;

/*
std::tuple<size_type, error_ptr> Copy(
    Writer_ptr dst,
    Reader_ptr src
);

std::tuple<size_type, error_ptr> CopyBuffer(
    Writer_ptr  dst,
    Reader_ptr  src,
    byte_vector buf
);

std::tuple<size_type, error_ptr> CopyN(
    Writer_ptr  dst,
    Reader_ptr  src,
    byte_vector buf
);

std::tuple<byte_vector, error_ptr> ReadAll(
    Reader_ptr r
);

class ByteReader
{
    virtual std::tuple<std::uint8_t, error_ptr> ReadByte() = 0;
};

class ByteScanner : public ByteReader
{
    virtual error UnreadByte() = 0;
};
*/

} // namespace wayround_i2p::akigo::io

#endif
