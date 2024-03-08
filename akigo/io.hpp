#ifndef WAYROUND_I2P_20240308_131206_300425
#define WAYROUND_I2P_20240308_131206_300425

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::io
{

using error       = wayround_i2p::akigo::builtin::error;
using size_type   = wayround_i2p::akigo::builtin::size_type;
using byte_vector = wayround_i2p::akigo::builtin::byte_vector;

class Reader
{
    virtual std::tuple<size_type, error> Read(byte_vector p) = 0;
};

using Reader_ptr = std::shared_ptr<Reader>;

class Writer
{
    virtual std::tuple<size_type, error> Write(byte_vector p) = 0;
};

using Writer_ptr = std::shared_ptr<Writer>;

std::tuple<size_type, error> Copy(
    Writer_ptr dst,
    Reader_ptr src
);

std::tuple<size_type, error> CopyBuffer(
    Writer_ptr  dst,
    Reader_ptr  src,
    byte_vector buf
);

std::tuple<size_type, error> CopyN(
    Writer_ptr  dst,
    Reader_ptr  src,
    byte_vector buf
);

std::tuple<byte_vector, error> ReadAll(
    Reader_ptr r
);

class ByteReader_nsp
{
    virtual std::tuple<std::uint8_t, error> ReadByte() = 0;
};

class ByteScanner_nsp : public ByteReader_nsp
{
    virtual error UnreadByte() = 0;
};

} // namespace wayround_i2p::akigo::io

#endif
