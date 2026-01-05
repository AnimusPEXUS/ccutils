
#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

consteval FDCtlInitOptions fdctl_normal_closed_options()
{
    return {
        .is_open                                                      = false,
        .close_on_destroy                                             = true,
        .guard_lower_functions_from_running_if_fdctl_in_closed_state  = true,
        .guard_higher_functions_from_running_if_fdctl_in_closed_state = true
    };
}

consteval FDCtlInitOptions fdctl_normal_open_options()
{
    auto ret    = fdctl_normal_closed_options();
    ret.is_open = true;
    return ret;
}

std::shared_ptr<FDCtl> FDCtl::create()
{
    return FDCtl::create(0, fdctl_normal_open_options());
}

std::shared_ptr<FDCtl> FDCtl::create(int fd)
{
    return FDCtl::create(fd, fdctl_normal_open_options());
}

std::shared_ptr<FDCtl> FDCtl::create(FDCtlInitOptions opts)
{
    return FDCtl::create(0, opts);
}

std::shared_ptr<FDCtl> FDCtl::create(int fd, FDCtlInitOptions opts)
{
    auto ret     = FDCtl_ptr(new FDCtl(fd, opts));
    ret->own_ptr = ret;
    return ret;
}

FDCtl::FDCtl(int fd, FDCtlInitOptions opts)
{
    this->opts = opts;
    this->fd   = fd;
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
    opts.is_open = false;

    int ret = ::close(this->fd);
    if (ret != 0)
    {
        return {ret, errno};
    }

    return {0, 0};
}

err_errNoS FDCtl::Close()
{
    if (opts.is_open)
    {
        return this->close();
    }
    else
    {
        return {0, 0};
    }
}

void FDCtl::setFD(int newfd, FDCtlInitOptions opts)
{
    this->fd   = newfd;
    this->opts = opts;
    return;
}

int FDCtl::getFD()
{
    return this->fd;
}

void FDCtl::setOpen(bool value)
{
    opts.is_open = value;
}

bool FDCtl::isOpen()
{
    return opts.is_open;
}

res_errNoS FDCtl::dup()
{
    res_errNoS ret = {0, 0};

    ret.res = ::dup(this->fd);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

res_errNoS FDCtl::dup2(int newfd)
{
    res_errNoS ret = {0, 0};

    ret.res = ::dup2(this->fd, newfd);
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

    ret.err = ::bind(this->fd, addr, length);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

err_errNoS FDCtl::getsockname(struct sockaddr *addr, socklen_t *length)
{
    err_errNoS ret = {0, 0};

    ret.err = ::getsockname(this->fd, addr, length);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

err_errNoS FDCtl::getpeername(struct sockaddr *addr, socklen_t *length)
{
    err_errNoS ret = {0, 0};

    ret.err = ::getpeername(this->fd, addr, length);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

res_errNoS FDCtl::connect(struct sockaddr *addr, socklen_t length)
{
    res_errNoS ret = {0, 0};

    ret.res = ::connect(this->fd, addr, length);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

err_errNoS FDCtl::listen(int n)
{
    err_errNoS ret = {0, 0};

    ret.err = ::listen(this->fd, n);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

res_errNoS FDCtl::accept(struct sockaddr *addr, socklen_t *length)
{
    res_errNoS ret = {0, 0};

    ret.res = ::accept(this->fd, addr, length);
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

    ret.res = ::ioctl(this->fd, request, args...);
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

    ret.res = ::fcntl(this->fd, cmd, args...);
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

    ret.res = ::getsockopt(this->fd, level, optname, optval, optlen);
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

    ret.res = ::setsockopt(this->fd, level, optname, optval, optlen);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

size_errNoS FDCtl::read(void *buffer, size_t size)
{
    size_errNoS ret = {0, 0};

    ret.size = ::read(this->fd, buffer, size);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

size_errNoS FDCtl::write(const void *buffer, size_t size)
{
    size_errNoS ret = {0, 0};

    ret.size = ::write(this->fd, buffer, size);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

size_errNoS FDCtl::send(const void *buffer, size_t size, int flags)
{
    size_errNoS ret = {0, 0};

    ret.size = ::send(this->fd, buffer, size, flags);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

size_errNoS FDCtl::recv(void *buffer, size_t size, int flags)
{
    size_errNoS ret = {0, 0};

    ret.size = ::recv(this->fd, buffer, size, flags);
    if (ret.not_ok())
    {
        ret.errNo = errno;
    }
    return ret;
}

FDCtl_res_errNoS FDCtl::Dup(FDCtlInitOptions opts)
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
            res.res,
            opts
        )
    };
}

FDCtl_res_errNoS FDCtl::Dup2(
    int              newfd,
    FDCtlInitOptions opts
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
            res.res,
            opts
        )
    };
}

/*
// note: see header for info on why this is disabled
FDCtl_ptr FDCtl::Dup2(FDCtl_ptr newfd)
{
}
*/

res_errNoS FDCtl::Socket(
    int domain,
    int type,
    int protocol
)
{
    return this->Socket(
        domain,
        type,
        protocol,
        fdctl_normal_closed_options(),
        false
    );
}

res_errNoS FDCtl::Socket(
    int              domain,
    int              type,
    int              protocol,
    FDCtlInitOptions opts,
    bool             dont_close_if_open
)
{
    // todo: additional checks
    if (opts.close_on_destroy && opts.is_open && !dont_close_if_open)
    {
        Close();
        // todo: report errors?
        // todo: yes - if close makes error - Socket() function should not progress
#warning "todo"
    }

    auto res = this->socket(domain, type, protocol);
    if (res.not_ok())
    {
        return res;
    }

    setFD(res.res, opts);

    return res;
}

err_errNoS FDCtl::Bind(FDAddress_ptr addr)
{
#warning "todo"
}

FDAddress_err_errNoS FDCtl::Get_X_Name(bool sock_or_peer)
{
    std::vector<std::uint8_t> tmp_addr_buff;
    tmp_addr_buff.clear();
    socklen_t getsockname_size = 0;

    err_errNoS err;

    if (!sock_or_peer)
    {
        err = this->getsockname(
            reinterpret_cast<sockaddr *>(tmp_addr_buff.data()),
            &getsockname_size
        );
    }
    else
    {
        err = this->getpeername(
            reinterpret_cast<sockaddr *>(tmp_addr_buff.data()),
            &getsockname_size
        );
    }

    if (err.not_ok())
    {
        return FDAddress_err_errNoS{err.err, err.errNo, nullptr};
    }

    tmp_addr_buff.assign(getsockname_size, 0);

    err = this->getsockname(
        reinterpret_cast<sockaddr *>(tmp_addr_buff.data()),
        &getsockname_size
    );
    if (err.not_ok())
    {
        return FDAddress_err_errNoS{err.err, err.errNo, nullptr};
    }

    auto fdaddr = FDAddress::create(tmp_addr_buff);

    return FDAddress_err_errNoS{0, 0, fdaddr};
}

FDAddress_err_errNoS FDCtl::GetSockName()
{
    return Get_X_Name(false);
}

FDAddress_err_errNoS FDCtl::GetPeerName()
{
    return Get_X_Name(true);
}

FDCtl_res_errNoS FDCtl::Connect(
    FDAddress_ptr addr
)
{
    return this->Connect(addr, fdctl_normal_open_options());
}

FDCtl_res_errNoS FDCtl::Connect(
    FDAddress_ptr    addr,
    FDCtlInitOptions opts
)
{
    auto addr_buff = addr->getAddrBuff();

    auto res = this->connect(
        reinterpret_cast<sockaddr *>(addr_buff.data()),
        addr_buff.size()
    );
    if (res.not_ok())
    {
        FDCtl_res_errNoS ret;
        ret.fdctl = nullptr;
        ret.res   = res.res;
        ret.errNo = res.errNo;
        return ret;
    }

    FDCtl_res_errNoS ret;

    ret.fdctl = FDCtl::create(res.res, opts);

    ret.res   = res.res;
    ret.errNo = 0;

    return ret;
}

err_errNoS FDCtl::Listen()
{
    // todo: something better for backlog parameter should be done. 10 constant for now.
    return this->listen(10);
}

err_errNoS FDCtl::Listen(int n)
{
    return this->listen(n);
}

FDCtl_FDAddress_res_errNoS FDCtl::Accept()
{
    return this->Accept(fdctl_normal_open_options());
}

FDCtl_FDAddress_res_errNoS FDCtl::Accept(FDCtlInitOptions opts)
{
    struct sockaddr addr;
    socklen_t       length;

    auto res = this->accept(&addr, &length);
    if (res.not_ok())
    {
        FDCtl_FDAddress_res_errNoS ret;
        ret.fdctl = nullptr;
        ret.addr  = nullptr;
        ret.res   = res.res;
        ret.errNo = res.errNo;
        return ret;
    }

    auto fd_addr = FDAddress::create(&addr, length);

    // todo: check fd_addr is ok and sane

    FDCtl_FDAddress_res_errNoS ret;

    ret.fdctl = FDCtl::create(res.res, opts);
    ret.addr  = fd_addr;

    ret.res   = res.res;
    ret.errNo = 0;

    return ret;
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

intval_res_errNoS FDCtl::getDomain()
{
    intval_res_errNoS ret;
    socklen_t         optlen;

    auto res = this->getsockopt(
        SOL_SOCKET,
        SO_DOMAIN,
        &ret.intval,
        &optlen
    );

    if (res.not_ok())
    {
        ret.intval = 0;
        ret.res    = res.res;
        ret.errNo  = res.errNo;
        return ret;
    }

    ret.res   = res.res;
    ret.errNo = 0;

    return ret;
}

intval_res_errNoS FDCtl::getType()
{
    intval_res_errNoS ret;
    socklen_t         optlen;

    auto res = this->getsockopt(
        SOL_SOCKET,
        SO_TYPE,
        &ret.intval,
        &optlen
    );

    if (res.not_ok())
    {
        ret.intval = 0;
        ret.res    = res.res;
        ret.errNo  = res.errNo;
        return ret;
    }

    ret.res   = res.res;
    ret.errNo = 0;

    return ret;
}

intval_res_errNoS FDCtl::getProtocol()
{
    intval_res_errNoS ret;
    socklen_t         optlen;

    auto res = this->getsockopt(
        SOL_SOCKET,
        SO_PROTOCOL,
        &ret.intval,
        &optlen
    );

    if (res.not_ok())
    {
        ret.intval = 0;
        ret.res    = res.res;
        ret.errNo  = res.errNo;
        return ret;
    }

    ret.res   = res.res;
    ret.errNo = 0;

    return ret;
}

domain_type_protocol_res_errNoS FDCtl::getDomainTypeProtocol()
{
    domain_type_protocol_res_errNoS ret;
    ret.domain   = 0;
    ret.type     = 0;
    ret.protocol = 0;
    ret.res      = 1; // error by default
    ret.errNo    = 1; // error by default

    auto domain   = getDomain();
    auto type     = getType();
    auto protocol = getProtocol();

    if (domain.res != 0)
    {
        ret.res   = domain.res;
        ret.errNo = domain.errNo;
        return ret;
    }

    if (type.res != 0)
    {
        ret.res   = type.res;
        ret.errNo = type.errNo;
        return ret;
    }

    if (protocol.res != 0)
    {
        ret.res   = protocol.res;
        ret.errNo = protocol.errNo;
        return ret;
    }

    ret.domain   = domain.intval;
    ret.type     = type.intval;
    ret.protocol = protocol.intval;
    ret.res      = 0;
    ret.errNo    = 0;

    return ret;
}

} // namespace wayround_i2p::ccutils::posix_tools
