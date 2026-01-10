#ifndef WAYROUND_I2P_20250205_113427_121004
#define WAYROUND_I2P_20250205_113427_121004

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/context.hpp>
#include <wayround_i2p/ccutils/akigo/os.hpp>
#include <wayround_i2p/ccutils/akigo/time.hpp>

namespace wayround_i2p::akigo::io
{

using error_ptr  = wayround_i2p::akigo::builtin::error_ptr;
using size_type  = wayround_i2p::akigo::builtin::size_type;
using byte_slice = wayround_i2p::akigo::builtin::byte_slice;
using ustring    = wayround_i2p::akigo::builtin::ustring;
using go_byte    = wayround_i2p::akigo::builtin::go_byte;
using go_rune    = wayround_i2p::akigo::builtin::go_rune;
using go_int     = wayround_i2p::akigo::builtin::go_int;
using go_uint    = wayround_i2p::akigo::builtin::go_uint;
using go_int64   = wayround_i2p::akigo::builtin::go_int64;
using go_uint64  = wayround_i2p::akigo::builtin::go_uint64;

class Deadlined
{
    virtual error_ptr SetDeadline(wayround_i2p::akigo::time::Time t)      = 0;
    virtual error_ptr SetReadDeadline(wayround_i2p::akigo::time::Time t)  = 0;
    virtual error_ptr SetWriteDeadline(wayround_i2p::akigo::time::Time t) = 0;
};

class Buffered
{
    // func (c *UnixConn) SetReadBuffer(bytes int) error
    virtual error_ptr SetReadBuffer(int bytes) = 0;

    // func (c *UnixConn) SetWriteBuffer(bytes int) error
    virtual error_ptr SetWriteBuffer(int bytes) = 0;
};

class Filed
{
    // func (c *UnixConn) File() (f *os.File, err error)
    virtual std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> File() = 0;
};

class Blockable
{
    virtual bool SupportsNonBlocking()      = 0;
    virtual bool GetNonBlocking()           = 0;
    virtual void SetNonBlocking(bool value) = 0;
};

class ByteReader
{
    // ReadByte() (byte, error)
    virtual std::tuple<go_byte, error_ptr> ReadByte() = 0;
};

class ByteScanner : public ByteReader
{
    // UnreadByte() error
    virtual error_ptr UnreadByte() = 0;
};

class ByteWriter
{
    // WriteByte(c byte) error
    virtual error_ptr WriteByte(go_byte c) = 0;
};

class Closer
{
    // Close() error
    virtual error_ptr Close() = 0;
};

class PartialCloser
{
    virtual error_ptr CloseRead()  = 0;
    virtual error_ptr CloseWrite() = 0;
};

class Reader
{
    // Read(p []byte) (n int, err error)
    virtual std::tuple<go_int, error_ptr> Read(byte_slice p) = 0;
};

using Reader_ptr = std::shared_ptr<Reader>;

class Writer
{
    // Write(p []byte) (n int, err error)
    virtual std::tuple<size_type, error_ptr> Write(byte_slice p) = 0;
};

class ReaderAt
{
    // ReadAt(p []byte, off int64) (n int, err error)
    virtual std::tuple<size_type, error_ptr> ReadAt(byte_slice p, go_int64 off) = 0;
};

class ReaderFrom
{
    // ReadFrom(r Reader) (n int64, err error)
    virtual std::tuple<go_int64, error_ptr> ReadFrom(Reader_ptr r) = 0;
};

class RuneReader
{
    // ReadRune() (r rune, size int, err error)
    virtual std::tuple<go_rune, go_int, error_ptr> ReadRune() = 0;
};

class RuneScanner : public RuneReader
{
    // UnreadRune() error
    virtual error_ptr UnreadRune() = 0;
};

class Seeker
{
    // Seek(offset int64, whence int) (int64, error)
    virtual std::tuple<go_int64, error_ptr> Seek(go_int64 offset, int whence) = 0;
};

class StringWriter
{
    // WriteString(s string) (n int, err error)
    virtual std::tuple<go_int, error_ptr> WriteString(ustring s) = 0;
};

class ReadCloser : public Reader,
                   public Closer
{
};

class ReadSeekCloser : public Reader,
                       public Seeker,
                       public Closer
{
};

class ReadSeeker : public Reader,
                   public Seeker
{
};

class ReadWriteCloser : public Reader,
                        public Writer,
                        public Closer
{
};

class ReadWriteSeeker : public Reader,
                        public Writer,
                        public Seeker
{
};

class ReadWriter : public Reader,
                   public Writer
{
};

class WriteCloser : public Writer, public Closer
{
};

class WriteSeeker : public Writer, public Seeker
{
};

} // namespace wayround_i2p::akigo::io

#endif
