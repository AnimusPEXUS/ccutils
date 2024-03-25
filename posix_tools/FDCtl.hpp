#ifndef WAYROUND_I2P_20240323_153956_244130
#define WAYROUND_I2P_20240323_153956_244130

#include <cstdint>
#include <functional>

#include <fcntl.h>
#include <sys/socket.h>
#include <time.h>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/errors.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

using error_ptr   = wayround_i2p::ccutils::posix_tools::error_ptr;
using byte_vector = wayround_i2p::ccutils::posix_tools::byte_vector;

class FDCtl
{
  private:
    int active_fd;

  public:
    FDCtl();
    FDCtl(int fd, bool close_on_destroy = false);

    ~FDCtl();

    int getFD();

    bool GetCloseOnDestory();
    void SetCloseOnDestory(bool);

    std::tuple<int, error_ptr> read(byte_vector buff);
    std::tuple<int, error_ptr> write(byte_vector buff);

    std::tuple<int, error_ptr> send(byte_vector buff);
    std::tuple<int, error_ptr> recv(byte_vector buff);

    // man 2 fcntl
    int fcntl(int, int cmd, ...);

    // man 2 getsockopt / man 2 setsockopt
    int getsockopt(int sockfd, int level, int optname, void optval[restrict *.optlen], socklen_t *restrict optlen);
    int setsockopt(int sockfd, int level, int optname, const void optval[.optlen], socklen_t optlen);

    // 0 on success
    int getRecvTimeouts(timeval &r);

    // 0 on success
    int getSendTimeouts(timeval &s);

    // 0 on success
    int setRecvTimeouts(timeval &r);

    // 0 on success
    int setSendTimeouts(timeval &s);

    // 0 on success
    int isNonBlocking(bool &ret);

    // 0 on success
    int setNonBlocking(bool blocking);

    int sendFixedData(
        std::uint8_t             *addr,
        std::size_t               count,
        std::function<bool()>     make_stop,
        typeof(timespec::tv_sec)  poll_sec,
        typeof(timespec::tv_nsec) poll_nsec
    );

    int recvFixedData(
        std::uint8_t             *addr,
        std::size_t               count,
        std::function<bool()>     make_stop,
        typeof(timespec::tv_sec)  poll_sec,
        typeof(timespec::tv_nsec) poll_nsec
    );

  private:
    bool close_on_destroy_;
};

} // namespace wayround_i2p::ccutils::posix_tools

#endif
