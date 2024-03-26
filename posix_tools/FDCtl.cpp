
#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

FDCtl::FDCtl(int fd, bool close_on_destroy)
{
    active_fd              = fd;
    this->close_on_destroy = close_on_destroy;
}

FDCtl::~FDCtl()
{
    if (!closed && close_on_destroy)
    {
        close(active_fd);
    }
}

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

} // namespace wayround_i2p::ccutils::posix_tools
