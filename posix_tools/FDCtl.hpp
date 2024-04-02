#ifndef WAYROUND_I2P_20240323_153956_244130
#define WAYROUND_I2P_20240323_153956_244130

#include <cstdint>
#include <functional>
#include <vector>

#include <fcntl.h>
#include <stddef.h>
#include <sys/socket.h>
#include <time.h>

namespace wayround_i2p::ccutils::posix_tools
{

using byte_vector = std::vector<uint8_t>;

class FDCtl
{
  private:
    int active_fd;

    bool closed;
    bool close_on_destroy;

    // std::weak_ptr<FDCtl> own_ptr;

  protected:
    FDCtl(int fd, bool close_on_destroy = false);

  public:
    static std::shared_ptr<FDCtl> create(
        int  fd,
        bool close_on_destroy = false
    );

    ~FDCtl();

    int close();

    int getFD();

    bool GetCloseOnDestory();
    void SetCloseOnDestory(bool);

    // gets address data from fd in this FDCtl and puts it into addr
    int setFDAddress(std::shared_ptr<FDAddress> addr);

    // v v v function direct forwardings v v v

    int dup();
    int dup2(int new);

    // warning:
    // note:
    // this function sets / replaces internal fd with new one and doesn't
    // do any additional actions. use Socket() function (below in this class)
    // if you want close_on_destroy value to be respected
    int socket(int namespace, int style, int protocol);

    int bind(struct sockaddr *addr, socklen_t length);
    int getsockname(struct sockaddr *addr, socklen_t *length);
    int connect(struct sockaddr *addr, socklen_t length);
    int listen(int n);
    int accept(struct sockaddr *addr, socklen_t *length_ptr);

    // man 2 ioctl
    template <typename... Args>
    int ioctl(unsigned long request, Args... args);

    // man 2 fcntl
    template <typename... Args>
    int fcntl(int, int cmd, Args... args);

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

    ssize_t read(void *buffer, size_t size);
    ssize_t write(const void *buffer, size_t size);

    ssize_t send(const void *buffer, size_t size, int flags);
    ssize_t recv(void *buffer, size_t size, int flags);

    // ^ ^ ^ function direct forwardings ^ ^ ^

    // v v v function shortcuts and usages v v v

    std::shared_ptr<FDCtl> Dup(bool close_on_destroy = false);

    std::shared_ptr<FDCtl> Dup2(int newfd, bool close_on_destroy = false);

    // note: making this function will introduce problem: this will
    //   require maintaining db and checking if result of dup2 is same as newfd
    // std::shared_ptr<FDCtl> Dup2(std::shared_ptr<FDCtl> newfd);

    // closes current fd (if it [is set] and [not closed]) and
    // calls socket() in this object's class with same parameters
    int Socket(int namespace, int style, int protocol);

    int Bind(std::shared_ptr<FDAddress> addr);

    std::tuple<std::shared_ptr<FDAddress>, int> GetSockName();

    int Connect(std::shared_ptr<FDAddress> addr);

    std::tuple<std::shared_ptr<FDAddress>, int> Accept();

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

    // 0 on success
    int getType(int &type);

    // ^ ^ ^ function shortcuts and usages ^ ^ ^

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
};

} // namespace wayround_i2p::ccutils::posix_tools

#endif
