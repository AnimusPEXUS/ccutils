
#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

std::shared_ptr<FDCtl> FDCtl::create(FDCtlInitOptions opts)
{
    auto ret     = std::shared_ptr<FDCtl>(new FDCtl(opts));
    ret->own_ptr = ret;
    return ret;
}

FDCtl::FDCtl(FDCtlInitOptions opts)
{
    this->opts = opts;
}

FDCtl::~FDCtl()
{
    if (opts.close_on_destroy)
    {
        this->close();
    }
}

err_errNoS FDCtl::close()
{
    opts.is_closed = true;

    int ret = ::close(opts.fd);
    if (ret != 0)
    {
        return {ret, errno};
    }

    return {0, 0};
}

err_errNoS FDCtl::Close()
{
    if (!opts.is_closed)
    {
        return this->close();
    }
    else
    {
        return {0, 0};
    }
}

int FDCtl::getFD()
{
    return opts.fd;
}

bool FDCtl::GetCloseOnDestroy()
{
    return opts.close_on_destroy;
}

void FDCtl::SetCloseOnDestroy(bool value)
{
    opts.close_on_destroy = value;
}

err_errNoS FDCtl::setFDAddress(std::shared_ptr<FDAddress> addr)
{
    if (!addr)
    {
        return {-3, 0};
    }

    std::vector<std::uint8_t> tmp_addr_buff;
    tmp_addr_buff.clear();
    socklen_t getsockname_size = 0;

    err_errNoS err = {0};

    err = this->getsockname(
        reinterpret_cast<sockaddr *>(tmp_addr_buff.data()),
        &getsockname_size
    );
    if (err.not_ok())
    {
        return err;
    }

    tmp_addr_buff.assign(getsockname_size, 0);

    err = this->getsockname(
        reinterpret_cast<sockaddr *>(tmp_addr_buff.data()),
        &getsockname_size
    );
    if (err.not_ok())
    {
        return err;
    }

    int err_int = addr->setAddrBuff(tmp_addr_buff);
    if (err_int != 0)
    {
        return {err_int, 0};
    }

    return {0, 0};
}

res_errNoS FDCtl::dup()
{
    res_errNoS ret = {0, 0};

    ret.res = ::dup(this->opts.fd);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

res_errNoS FDCtl::dup2(int newfd)
{
    res_errNoS ret = {0, 0};

    ret.res = ::dup2(this->opts.fd, newfd);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

res_errNoS FDCtl::socket(int domain, int type, int protocol)
{
    res_errNoS ret = {0, 0};

    ret.res = ::socket(domain, type, protocol);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

err_errNoS FDCtl::bind(struct sockaddr *addr, socklen_t length)
{
    err_errNoS ret = {0, 0};

    ret.err = ::bind(this->opts.fd, addr, length);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

err_errNoS FDCtl::getsockname(struct sockaddr *addr, socklen_t *length)
{
    err_errNoS ret = {0, 0};

    ret.err = ::getsockname(this->opts.fd, addr, length);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

err_errNoS FDCtl::connect(struct sockaddr *addr, socklen_t length)
{
    err_errNoS ret = {0, 0};

    ret.err = ::connect(this->opts.fd, addr, length);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

err_errNoS FDCtl::listen(int n)
{
    err_errNoS ret = {0, 0};

    ret.err = ::listen(this->opts.fd, n);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

err_errNoS FDCtl::accept(struct sockaddr *addr, socklen_t *length)
{
    err_errNoS ret = {0, 0};

    ret.err = ::accept(this->opts.fd, addr, length);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

template <typename... Args>
res_errNoS FDCtl::ioctl(unsigned long request, Args... args)
{
    res_errNoS ret = {0, 0};

    ret.res = ::ioctl(this->opts.fd, request, args...);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

template <typename... Args>
res_errNoS FDCtl::fcntl(int cmd, Args... args)
{
    res_errNoS ret = {0, 0};

    ret.res = ::fcntl(this->opts.fd, cmd, args...);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

res_errNoS FDCtl::getsockopt(
    int        level,
    int        optname,
    void      *optval,
    socklen_t *optlen
)
{
    res_errNoS ret = {0, 0};

    ret.res = ::getsockopt(this->opts.fd, level, optname, optval, optlen);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

res_errNoS FDCtl::setsockopt(
    int         level,
    int         optname,
    const void *optval,
    socklen_t   optlen
)
{
    res_errNoS ret = {0, 0};

    ret.res = ::setsockopt(this->opts.fd, level, optname, optval, optlen);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

size_errNoS FDCtl::read(void *buffer, size_t size)
{
    size_errNoS ret = {0, 0};

    ret.size = ::read(this->opts.fd, buffer, size);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

size_errNoS FDCtl::write(const void *buffer, size_t size)
{
    size_errNoS ret = {0, 0};

    ret.size = ::write(this->opts.fd, buffer, size);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

size_errNoS FDCtl::send(const void *buffer, size_t size, int flags)
{
    size_errNoS ret = {0, 0};

    ret.size = ::send(this->opts.fd, buffer, size, flags);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

size_errNoS FDCtl::recv(void *buffer, size_t size, int flags)
{
    size_errNoS ret = {0, 0};

    ret.size = ::recv(this->opts.fd, buffer, size, flags);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

FDCtl_res_errNoS FDCtl::Dup(bool close_on_destroy)
{
    res_errNoS res = this->dup();
    if (res.not_ok())
    {
        return FDCtl_res_errNoS{
            res,
            nullptr
        };
    }

    return {
        res,
        FDCtl::create(
            {.fd               = res.res,
             .is_closed        = false,
             .close_on_destroy = true
            }
        )
    };
}

FDCtl_res_errNoS FDCtl::Dup2(
    int  newfd,
    bool close_on_destroy
)
{
    res_errNoS res = this->dup2(newfd);
    if (res.not_ok())
    {
        return FDCtl_res_errNoS{
            res,
            nullptr
        };
    }

    return {
        res,
        FDCtl::create(
            {.fd               = res.res,
             .is_closed        = false,
             .close_on_destroy = true
            }
        )
    };
}

/*
// note: see header for info on why this is disabled
std::shared_ptr<FDCtl> FDCtl::Dup2(std::shared_ptr<FDCtl> newfd)
{
}
*/

res_errNoS FDCtl::Socket(int domain, int type, int protocol)
{
    // todo: additional checks
    return this->socket(domain, type, protocol);
}

res_errNoS FDCtl::getRecvTimeout(timeval &r)
{
    // todo: questionable. testing required. is this correct acquision of
    //       getsockopt results ?
    socklen_t res_optlen = 0;

    auto res = this->getsockopt(SOL_SOCKET, SO_RCVTIMEO, &r, &res_optlen);
    if (res.not_ok())
    {
        return res;
    }

    // todo: additional checks?

    return res;
}

res_errNoS FDCtl::getSendTimeout(timeval &s)
{

    // todo: questionable. testing required. is this correct acquision of
    //       getsockopt results ?
    socklen_t res_optlen = 0;

    auto res = this->getsockopt(SOL_SOCKET, SO_SNDTIMEO, &s, &res_optlen);
    if (res.not_ok())
    {
        return res;
    }

    // todo: additional checks?

    return res;
}

res_errNoS FDCtl::setRecvTimeout(timeval &r)
{
    auto res = this->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &r, sizeof(r));
    if (res.not_ok())
    {
        return res;
    }

    // todo: additional checks?

    return res;
}

res_errNoS FDCtl::setSendTimeout(timeval &s)
{
    auto res = this->setsockopt(SOL_SOCKET, SO_SNDTIMEO, &s, sizeof(s));
    if (res.not_ok())
    {
        return res;
    }

    // todo: additional checks?

    return res;
}

res_errNoS FDCtl::isNonBlocking(bool &ret)
{
    // note: if better function for work with fcntl appears - use it

    res_errNoS res = this->fcntl(F_GETFL, 0);
    if (res.not_ok())
    {
        return res;
    }

    ret = (res.res & O_NONBLOCK) != 0;

    return res;
}

res_errNoS FDCtl::setNonBlocking(bool blocking)
{
    // note: if better function for work with fcntl appears - use it

    res_errNoS res = this->fcntl(F_GETFL, 0);
    if (res.not_ok())
    {
        return res;
    }

    if (blocking)
    {
        res.res |= O_NONBLOCK;
    }
    else
    {
        res.res &= !O_NONBLOCK;
    }

    res = this->fcntl(F_SETFL, res);
    if (res.not_ok())
    {
        return res;
    }

    return res;
}

res_errNoS FDCtl::getType(int &type)
{
    socklen_t optlen;

    return this->getsockopt(
        SOL_SOCKET,
        SO_TYPE,
        &type,
        &optlen
    );
}

} // namespace wayround_i2p::ccutils::posix_tools
