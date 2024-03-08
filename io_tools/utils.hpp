
#include <cstdint>
#include <functional>

#include <fcntl.h>
#include <sys/socket.h>
#include <time.h>

namespace wayround_i2p::ccutils::io_tools
{

// 0 on success
int getRecvTimeouts(int fd, timeval &r);

// 0 on success
int getSendTimeouts(int fd, timeval &s);

// 0 on success
int setRecvTimeouts(int fd, timeval &r);

// 0 on success
int setSendTimeouts(int fd, timeval &s);

// 0 on success
int isNonBlocking(int fd, bool &ret);

// 0 on success
int setNonBlocking(int fd, bool blocking);

/*
int sendFixedData(
    int                       fd,
    std::uint8_t             *addr,
    std::size_t               count,
    std::function<bool()>     make_stop,
    typeof(timespec::tv_sec)  poll_sec,
    typeof(timespec::tv_nsec) poll_nsec
);

int recvFixedData(
    int                       fd,
    std::uint8_t             *addr,
    std::size_t               count,
    std::function<bool()>     make_stop,
    typeof(timespec::tv_sec)  poll_sec,
    typeof(timespec::tv_nsec) poll_nsec
);
*/

} // namespace wayround_i2p::ccutils::io_tools
