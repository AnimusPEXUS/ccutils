
#include <wayround_i2p/ccutils/io_tools/utils.hpp>

namespace wayround_i2p::ccutils::io_tools
{

int getRecvTimeout(int fd, timeval &r)
{
    int err = 0;

    err = getsockopt(SOL_SOCKET, SO_RCVTIMEO, &r, sizeof(timeval));
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int getSendTimeout(int fd, timeval &s)
{
    int err = 0;

    err = getsockopt(SOL_SOCKET, SO_SNDTIMEO, &s, sizeof(timeval));
    if (err != 0)
    {
        return err;
    }
    return 0;
}

int setRecvTimeout(int fd, timeval &r)
{
    int err = 0;

    err = getsockopt(SOL_SOCKET, SO_RCVTIMEO, &r, sizeof(timeval));
    if (err != 0)
    {
        return err;
    }

    return 0;
}

int setSendTimeout(int fd, timeval &s)
{
    int err = 0;

    err = setsockopt(SOL_SOCKET, SO_SNDTIMEO, &s, sizeof(timeval));
    if (err != 0)
    {
        return err;
    }
    return 0;
}

int isNonBlocking(int fd, bool &ret)
{
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

} // namespace wayround_i2p::ccutils::io_tools
