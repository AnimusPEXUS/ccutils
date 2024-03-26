#ifndef WAYROUND_I2P_20240323_153956_244130
#define WAYROUND_I2P_20240323_153956_244130

#include <cstdint>
#include <functional>

#include <fcntl.h>
#include <stddef.h>
#include <sys/socket.h>
#include <time.h>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/errors.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

using error_ptr   = wayround_i2p::akigo::builtin::error_ptr;
using byte_vector = wayround_i2p::akigo::builtin::byte_vector;

class FDCtl
{
  private:
    int active_fd;

    bool closed;
    bool close_on_destroy;

  public:
    FDCtl(int fd, bool close_on_destroy = false);

    ~FDCtl();

    int getFD();

    bool GetCloseOnDestory();
    void SetCloseOnDestory(bool);

    // v v v function direct forwardings v v v

    ssize_t read(void *buffer, size_t size);
    ssize_t write(const void *buffer, size_t size);

    ssize_t send(const void *buffer, size_t size, int flags);
    ssize_t recv(void *buffer, size_t size, int flags);

    // man 2 fcntl
    int fcntl(int, int cmd, ...);

    // see man 2 getsockopt / man 2 setsockopt / man 7 socket
    // (for options and their docs)
    int getsockopt(
        int        level,
        int        optname,
        void      *optval,
        socklen_t *optlen
    );

    int setsockopt(
        int         level,
        int         optname,
        const void *optval,
        socklen_t   optlen
    );

    // ^ ^ ^ function direct forwardings ^ ^ ^

    // v v v function shortcuts and usages v v v

    // 0 on success
    int getRecvTimeout(timeval &r);

    // 0 on success
    int getSendTimeout(timeval &s);

    // 0 on success
    int setRecvTimeout(timeval &r);

    // 0 on success
    int setSendTimeout(timeval &s);

    // 0 on success
    int isNonBlocking(bool &ret);

    // 0 on success
    int setNonBlocking(bool blocking);

    int sendFixedData(
        byte_vector              &buff,
        std::function<bool()>     make_stop,
        typeof(timespec::tv_sec)  poll_sec,
        typeof(timespec::tv_nsec) poll_nsec
    );

    int recvFixedData(
        byte_vector              &buff,
        std::function<bool()>     make_stop,
        typeof(timespec::tv_sec)  poll_sec,
        typeof(timespec::tv_nsec) poll_nsec
    );

    // ^ ^ ^ function shortcuts and usages ^ ^ ^
};

} // namespace wayround_i2p::ccutils::posix_tools

#endif
