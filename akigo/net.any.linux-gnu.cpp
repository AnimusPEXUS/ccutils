
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
    auto res = fdctl->Read(p);
    if (!res.is_ok())
    {
        return {
            -1,
            wayround_i2p::akigo::errors::New(
                "reading from file descriptor has failed",
                __FILE__,
                __LINE__
            )
        };
    }

    return {res.size, nullptr};
}

std::tuple<go_int, error_ptr> LocalConn::Write(byte_slice p)
{
    auto res = fdctl->Write(p);
    if (!res.is_ok())
    {
        return {
            -1,
            wayround_i2p::akigo::errors::New(
                "writing to file descriptor has failed",
                __FILE__,
                __LINE__
            )
        };
    }

    return {res.size, nullptr};
}

error_ptr LocalConn::Close()
{
    is_open  = false;
    auto res = fdctl->Close();

    if (!res.is_ok())
    {
        return wayround_i2p::akigo::errors::New(
            std::format("Couldn't close fdctl"),
            __FILE__,
            __LINE__
        );
    }

    return nullptr;
}

error_ptr LocalConn::CloseRead()
{
    #warning "todo"
    return {
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

error_ptr LocalConn::CloseWrite()
{
    #warning "todo"
    return {
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

error_ptr LocalConn::SetDeadline(wayround_i2p::akigo::time::Time t)
{
#warning "todo"
    return {
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

error_ptr LocalConn::SetReadDeadline(wayround_i2p::akigo::time::Time t)
{
#warning "todo"
    return {
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

error_ptr LocalConn::SetWriteDeadline(wayround_i2p::akigo::time::Time t)
{
#warning "todo"
    return {
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

std::tuple<go_int64, error_ptr> LocalConn::WriteTo(wayround_i2p::akigo::io::Writer_ptr w)
{
#warning "todo"
    return {
        -1,
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

std::tuple<go_int64, error_ptr> LocalConn::ReadFrom(wayround_i2p::akigo::io::Reader_ptr r)
{
#warning "todo"
    return {
        -1,
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

error_ptr LocalConn::SetReadBuffer(int bytes)
{
#warning "todo"
    return {
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

error_ptr LocalConn::SetWriteBuffer(int bytes)
{
#warning "todo"
    return {
        wayround_i2p::akigo::errors::New(
            std::format("todo"),
            __FILE__,
            __LINE__
        )
    };
}

} // namespace wayround_i2p::akigo::net
