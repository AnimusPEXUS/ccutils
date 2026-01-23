
#include <wayround_i2p/ccutils/akigo/net.hpp>
#include <wayround_i2p/ccutils/akigo/time.hpp>

namespace wayround_i2p::akigo::net
{

LocalConn::LocalConn(
    wayround_i2p::ccutils::posix_tools::FDCtl_ptr fdctl
) : fdctl(fdctl), is_open(is_open) /* todo: fix clang-format so initializers each on own line */
{
}

LocalConn::~LocalConn()
{
}

std::tuple<wayround_i2p::akigo::os::File_ptr, error_ptr> LocalConn::File()
{
#warning "todo"
    return {
        nullptr,
        wayround_i2p::akigo::errors::New(
            "todo",
            __FILE__,
            __LINE__
        )
    };
}

std::tuple<go_int, error_ptr> LocalConn::Read(byte_slice p)
{
	fdctl->
}

std::tuple<go_int, error_ptr> LocalConn::Write(byte_slice p)
{
}

error_ptr LocalConn::Close()
{
    is_open  = false;
    auto res = fdctl->Close();

    auto err = res.err;
    if (err)
    {
        return err;
    }

    return nullptr;
}

error_ptr LocalConn::SetDeadline(wayround_i2p::akigo::time::Time t)
{
}

error_ptr LocalConn::SetReadDeadline(wayround_i2p::akigo::time::Time t)
{
}

error_ptr LocalConn::SetWriteDeadline(wayround_i2p::akigo::time::Time t)
{
}

std::tuple<go_int64, error_ptr> LocalConn::WriteTo(wayround_i2p::akigo::io::Writer_ptr w)
{
}

std::tuple<go_int64, error_ptr> LocalConn::ReadFrom(wayround_i2p::akigo::io::Reader_ptr r)
{
}

error_ptr LocalConn::SetReadBuffer(int bytes)
{
}

error_ptr LocalConn::SetWriteBuffer(int bytes)
{
}

Addr_ptr LocalConn::LocalAddr()
{
}

Addr_ptr LocalConn::RemoteAddr()
{
}

} // namespace wayround_i2p::akigo::net
