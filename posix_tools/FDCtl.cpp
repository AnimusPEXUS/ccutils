
#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

std::shared_ptr<FDCtl> FDCtl::create(int fd, bool close_on_destroy)
{
    auto ret = std::shared_ptr<FDCtl>(new FDCtl(fd, close_on_destroy));
    // ret->own_ptr = ret;
    return ret;
}

FDCtl::FDCtl(int fd, bool close_on_destroy)
{
    active_fd              = fd;
    this->close_on_destroy = close_on_destroy;
}

FDCtl::~FDCtl()
{
    if (close_on_destroy)
    {
        this->close();
    }
}

int FDCtl::close()
{
    int ret = 0;
    if (!closed)
    {
        ret    = ::close(active_fd);
        closed = true;
    }
    return ret;
}

int FDCtl::getFD()
{
    return active_fd;
}

bool FDCtl::GetCloseOnDestory()
{
    return close_on_destroy;
}

void FDCtl::SetCloseOnDestory(bool value)
{
    close_on_destroy = value;
}

int FDAddress::setFDAddress(std::shared_ptr<FDAddress> addr)
{
    if (!fd)
    {
        return -3;
    }

    std::vector<std::uint8_t> tmp_addr_buff;
    tmp_addr_buff.clear();
    socklen_t getsockname_size;

    int err = 0;

    err = temp_fd->getsockname(tmp_addr_buff.data, &getsockname_size);
    if (err != 0)
    {
        return err;
    }

    tmp_addr_buff.assign(getsockname_size, 0);

    err = temp_fd->getsockname(tmp_addr_buff.data, &getsockname_size);
    if (err != 0)
    {
        return err;
    }

    err = fd->setAddrBuff(tmp_addr_buff);
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int FDAddress::reGetAddress()
{
    if (auto temp_fd = this->fd.lock(); temp_fd)
    {
        this->addr_buff.clear();

        this->addr_buff = tmp_addr_buff;
    }
    else
    {
        return -1;
    }

    return 0;
}

int FDCtl::dup()
{
    return ::dup(this->active_fd);
}

int FDCtl::dup2(int new)
{
    return ::dup2(this->active_fd, new);
}

int FDCtl::socket(int namespace, int style, int protocol)
{
    return ::socket(namespace, style, protocol);
}

int FDCtl::bind(struct sockaddr *addr, socklen_t length)
{
    return ::bind(this->active_fd, addr, length);
}

int FDCtl::getsockname(struct sockaddr *addr, socklen_t *length)
{
    return ::getsockname(this->active_fd, addr, length);
}

int FDCtl::connect(struct sockaddr *addr, socklen_t length)
{
    return ::connect(this->active_fd, addr, length);
}

int FDCtl::listen(int n)
{
    return ::listen(this->active_fd, n);
}

int FDCtl::accept(struct sockaddr *addr, socklen_t *length)
{
    return ::accept(this->active_fd, addr, length);
}

template <typename... Args>
int FDCtl::ioctl(unsigned long request, Args... args)
{
    return ::ioctl(this->active_fd, request, args...);
}

template <typename... Args>
int FDCtl::fcntl(int cmd, Args... args)
{
    return ::fcntl(this->active_fd, cmd, args...);
}

int FDCtl::getsockopt(
    int        level,
    int        optname,
    void      *optval,
    socklen_t *optlen
)
{
    return ::getsocket(level, optname, optval, optlen);
}

int FDCtl::setsockopt(
    int         level,
    int         optname,
    const void *optval,
    socklen_t   optlen
)
{
    return ::setsocket(level, optname, optval, optlen);
}

ssize_t FDCtl::read(void *buffer, size_t size)
{
    return ::read(this->active_fd, buffer, size);
}

ssize_t FDCtl::write(const void *buffer, size_t size)
{
    return ::write(this->active_fd, buffer, size);
}

ssize_t FDCtl::send(const void *buffer, size_t size, int flags)
{
    return ::send(this->active_fd, buffer, size, flags);
}

ssize_t FDCtl::recv(void *buffer, size_t size, int flags)
{
    return ::recv(this->active_fd, buffer, size, flags);
}

std::tuple(std::shared_ptr<FDCtl>, int) Dup(bool close_on_destroy = false)
{
    int ret = this->dup();
    if (ret < 0)
    {
        return std::tuple(nullptr, ret);
    }

    return std::tuple(this->create(ret, close_on_destroy), 0);
}

std::tuple(std::shared_ptr<FDCtl>, int) Dup2(int newfd, bool close_on_destroy = false)
{
    int ret = this->dup2(new);
    if (ret < 0)
    {
        return std::tuple(nullptr, ret);
    }

    return std::tuple(this->create(ret, close_on_destroy), 0);
}

/*
// note: see header for info on why this is disabled
std::shared_ptr<FDCtl> Dup2(std::shared_ptr<FDCtl> newfd)
{
}
*/

int FDCtl::getRecvTimeout(timeval &r)
{
    int err = 0;

    // todo: questionable. testing required. is this correct acquision of
    //       getsockopt results ?
    socklen_t res_optlen = 0;

    err = this->getsockopt(SOL_SOCKET, SO_RCVTIMEO, &r, &res_optlen);
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int FDCtl::getSendTimeout(timeval &s)
{
    int err = 0;

    // todo: questionable. testing required. is this correct acquision of
    //       getsockopt results ?
    socklen_t res_optlen = 0;

    err = this->getsockopt(SOL_SOCKET, SO_SNDTIMEO, &s, &res_optlen);
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int FDCtl::setRecvTimeout(timeval &r)
{
    int err = 0;

    err = this->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &r, sizeof(r));
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int FDCtl::setSendTimeout(timeval &s)
{
    int err = 0;

    err = this->setsockopt(SOL_SOCKET, SO_SNDTIMEO, &s, sizeof(s));
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int FDCtl::isNonBlocking(bool &ret)
{
    // note: if better function for work with fcntl appears - use it

    int res = this->fcntl(F_GETFL, 0);
    if (res == -1)
    {
        return -1;
    }
    ret = (res & O_NONBLOCK) != 0;
    return 0;
}

int FDCtl::setNonBlocking(bool blocking)
{
    // note: if better function for work with fcntl appears - use it

    int res = this->fcntl(F_GETFL, 0);
    if (res == -1)
    {
        return -1;
    }

    if (blocking)
    {
        res |= O_NONBLOCK;
    }
    else
    {
        res &= !O_NONBLOCK;
    }

    res = this->fcntl(F_SETFL, res);
    if (res == -1)
    {
        return -1;
    }

    return 0;
}

int FDCtl::getType(int &type)
{
    return this->getsockopt(SOL_SOCKET, SO_TYPE, &type, sizeof(int));
}

} // namespace wayround_i2p::ccutils::posix_tools
