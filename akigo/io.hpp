#ifndef WAYROUND_I2P_20260131_182305_0000231208
#define WAYROUND_I2P_20260131_182305_0000231208

#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/context.hpp>
#include <wayround_i2p/ccutils/akigo/os.hpp>
#include <wayround_i2p/ccutils/akigo/time.hpp>

namespace wayround_i2p::akigo::io
{

CCUTILS_MACRO_USE_TYPES_SET00

class Deadlined
{
    virtual error_ptr SetDeadline(wayround_i2p::akigo::time::Time t) = 0;
};

class DeadlinedReadWriter
    : public virtual Deadlined
{
    virtual error_ptr SetReadDeadline(wayround_i2p::akigo::time::Time t)  = 0;
    virtual error_ptr SetWriteDeadline(wayround_i2p::akigo::time::Time t) = 0;
};

class Filed
{
  public:
    // func (c *UnixConn) File() (f *os.File, err error)
    virtual std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> File() = 0;
};

// note: Blockable class/interface is not a part of Golang
class Blockable
{
  public:
    virtual bool SupportsNonBlocking()      = 0;
    virtual bool GetNonBlocking()           = 0;
    virtual void SetNonBlocking(bool value) = 0;
};

class ByteReader
{
  public:
    // ReadByte() (byte, error)
    virtual std::tuple<go_byte, error_ptr> ReadByte() = 0;
};

class ByteScanner
    : public virtual ByteReader
{
  public:
    // UnreadByte() error
    virtual error_ptr UnreadByte() = 0;
};

class ByteWriter
{
  public:
    // WriteByte(c byte) error
    virtual error_ptr WriteByte(go_byte c) = 0;
};

class Closer
{
  public:
    // Close() error
    virtual error_ptr Close() = 0;
};

class Reader
{
  public:
    // Read(p []byte) (n int, err error)
    virtual std::tuple<go_int, error_ptr> Read(byte_slice p) = 0;
};

using Reader_ptr = std::shared_ptr<Reader>;

class Writer
{
  public:
    // Write(p []byte) (n int, err error)
    virtual std::tuple<go_int, error_ptr> Write(byte_slice p) = 0;
};

using Writer_ptr = std::shared_ptr<Writer>;

class ReaderAt
{
  public:
    // ReadAt(p []byte, off int64) (n int, err error)
    virtual std::tuple<size_type, error_ptr> ReadAt(byte_slice p, go_int64 off) = 0;
};

class ReaderFrom
{
  public:
    // ReadFrom(r Reader) (n int64, err error)
    virtual std::tuple<go_int64, error_ptr> ReadFrom(Reader_ptr r) = 0;
};

class WriterAt
{
  public:
    // WriteAt(p []byte, off int64) (n int, err error)
    virtual std::tuple<go_int, error_ptr> WriteAt(byte_slice p, go_int off) = 0;
};

class WriterTo
{
  public:
    // WriteTo(w Writer) (n int64, err error)
    virtual std::tuple<go_int64, error_ptr> WriteTo(Writer_ptr w) = 0;
};

class RuneReader
{
  public:
    // ReadRune() (r rune, size int, err error)
    virtual std::tuple<go_rune, go_int, error_ptr> ReadRune() = 0;
};

class RuneScanner
    : public virtual RuneReader
{
  public:
    // UnreadRune() error
    virtual error_ptr UnreadRune() = 0;
};

class Seeker
{
  public:
    // Seek(offset int64, whence int) (int64, error)
    virtual std::tuple<go_int64, error_ptr> Seek(go_int64 offset, int whence) = 0;
};

class StringWriter
{
  public:
    // WriteString(s string) (n int, err error)
    virtual std::tuple<go_int, error_ptr> WriteString(ustring s) = 0;
};

class ReadCloser
    : public virtual Reader
    , public virtual Closer
{
};

class ReadSeekCloser
    : public virtual Reader
    , public virtual Seeker
    , public virtual Closer
{
};

class ReadSeeker
    : public virtual Reader
    , public virtual Seeker
{
};

class ReadWriteCloser
    : public virtual Reader
    , public virtual Writer
    , public virtual Closer
{
};

class ReadWriteSeeker
    : public virtual Reader
    , public virtual Writer
    , public virtual Seeker
{
};

class ReadWriter
    : public virtual Reader
    , public virtual Writer
{
};

class ReadWriterFromTo
    : public virtual ReaderFrom
    , public virtual WriterTo
{
};

class WriteCloser
    : public virtual Writer
    , public virtual Closer
{
};

class WriteSeeker
    : public virtual Writer
    , public virtual Seeker
{
};

} // namespace wayround_i2p::akigo::io

#endif
