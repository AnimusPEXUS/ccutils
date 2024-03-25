
#include <wayround_i2p/ccutils/posix_tools/FDCtl.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

int getRecvTimeout(int fd, timeval &r)
{
    int err = 0;

    // todo: questionable. testing required. is this correct acquision of
    //       getsockopt results ?
    socklen_t res_optlen = 0;

    err = getsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &r, &res_optlen);
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int getSendTimeout(int fd, timeval &s)
{
    int err = 0;

    // todo: questionable. testing required. is this correct acquision of
    //       getsockopt results ?
    socklen_t res_optlen = 0;

    err = getsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &s, &res_optlen);
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int setRecvTimeout(int fd, timeval &r)
{
    int err = 0;

    err = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &r, sizeof(r));
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int setSendTimeout(int fd, timeval &s)
{
    int err = 0;

    err = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &s, sizeof(s));
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int isNonBlocking(int fd, bool &ret)
{
    // note: if better function for work with fcntl appears - use it

    int res = fcntl(fd, F_GETFL, 0);
    if (res == -1)
    {
        return -1;
    }
    ret = (res & O_NONBLOCK) != 0;
    return 0;
}

int setNonBlocking(int fd, bool blocking)
{
    // note: if better function for work with fcntl appears - use it

    int res = fcntl(fd, F_GETFL, 0);
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

    res = fcntl(fd, F_SETFL, res);
    if (res == -1)
    {
        return -1;
    }
    return 0;
}

} // namespace wayround_i2p::ccutils::posix_tools
