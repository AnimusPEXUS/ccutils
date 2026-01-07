#ifndef WAYROUND_I2P_20250205_113427_121004
#define WAYROUND_I2P_20250205_113427_121004

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>

namespace wayround_i2p::akigo::io
{

using error_ptr   = wayround_i2p::akigo::builtin::error_ptr;
using size_type   = wayround_i2p::akigo::builtin::size_type;
using byte_vector = wayround_i2p::akigo::builtin::byte_vector;
using byte        = uint8_t;

class ByteReader
{
    // ReadByte() (byte, error)
    virtual std::tuple<byte, error_ptr> ReadByte() = 0;
};

class ByteScanner : public ByteReader
{
    // UnreadByte() error
    virtual error UnreadByte() = 0;
};

class ByteWriter
{
    // WriteByte(c byte) error
    virtual error_ptr WriteByte(byte c) = 0;
};

class Closer
{
    // Close() error
    virtual error_ptr Close() = 0;
};

class ReadCloser : public Reader, public Closer
{
};

class ReadSeekCloser : public Reader, public Seeker, public Closer
{
};

class ReadSeeker : public Reader, public Seeker
{
};

class ReadWriteCloser : public Reader, public Writer, public Closer
{
};

} // namespace wayround_i2p::akigo::io

#endif
