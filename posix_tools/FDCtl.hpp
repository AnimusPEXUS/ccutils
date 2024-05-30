#ifndef WAYROUND_I2P_20240527_064240_390320
#define WAYROUND_I2P_20240527_064240_390320

#include <cerrno>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include <fcntl.h>
#include <stddef.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <time.h>

#include <wayround_i2p/ccutils/posix_tools/FDAddress.hpp>

namespace wayround_i2p::ccutils::posix_tools
{

using byte_vector = std::vector<uint8_t>;

class FDCtl;

// class FDAddress;
// int FDAddress::setAddrBuff(std::vector<std::uint8_t> addr_buff);

struct errS
{
    int  err = -1; // real func result
    bool not_ok()
    {
        return err != 0;
    }
};

struct resS
{
    int  res = -1; // real func result
    bool not_ok()
    {
        return res == -1;
    }
};

struct sizeS
{
    int  size = -1; // real func result
    bool not_ok()
    {
        return size == -1;
    }
};

struct errNoS
{
    int errNo = 1; // errno on func error
};

struct err_errNoS : errS, errNoS
{
};

struct res_errNoS : resS, errNoS
{
};

struct size_errNoS : sizeS, errNoS
{
};

struct FDCtl_res_errNoS : res_errNoS
{
    std::shared_ptr<FDCtl> fdctl;
};

struct FDAddress_err_errNoS : err_errNoS
{
    std::shared_ptr<FDAddress> addr;
};

struct FDCtl_FDAddress_res_errNoS : res_errNoS
{
    std::shared_ptr<FDCtl>     fdctl;
    std::shared_ptr<FDAddress> addr;
};

struct socktype_res_errNoS : res_errNoS
{
    int type;
};

struct FDCtlInitOptions
{
    bool is_open          = false;
    bool close_on_destroy = false;

    // todo: make funcs use those
    bool guard_lower_functions_from_running_if_closed  = false;
    bool guard_higher_functions_from_running_if_closed = false;
};

consteval FDCtlInitOptions fdctl_normal_closed_options();
consteval FDCtlInitOptions fdctl_normal_open_options();

class FDCtl
{
  private:
    std::weak_ptr<FDCtl> own_ptr;

  protected:
    FDCtl(int fd, FDCtlInitOptions opts);

  public:
    static std::shared_ptr<FDCtl> create(int fd, FDCtlInitOptions opts);

    ~FDCtl();

    int              effective_fd = 0;
    FDCtlInitOptions opts;

    // does direct call without any checks and/or precautions.
    // in any case this puts FDCtl into closed state.
    err_errNoS close();

    // does internal checks and if everything's ok - calls this->close().
    // if already closed - this is not error.
    err_errNoS Close();

    void setFD(int newfd, FDCtlInitOptions opts);
    int  getFD();

    bool isOpen();
    void setOpen(bool value);

    // -----------------------------------------------------
    // v v v function direct forwardings v v v (lower functions)
    // -----------------------------------------------------

    res_errNoS dup();
    res_errNoS dup2(int newfd);

    // warning:
    // note:
    // this function sets / replaces internal fd with new one and doesn't
    // do any additional actions. use Socket() function (below in this class)
    // if you want close_on_destroy value to be respected
    res_errNoS socket(int domain, int type, int protocol);

    err_errNoS bind(struct sockaddr *addr, socklen_t length);
    err_errNoS getsockname(struct sockaddr *addr, socklen_t *length);
    err_errNoS getpeername(struct sockaddr *addr, socklen_t *length);
    res_errNoS connect(struct sockaddr *addr, socklen_t length);
    err_errNoS listen(int n);
    res_errNoS accept(struct sockaddr *addr, socklen_t *length_ptr);

    // man 2 ioctl
    template <typename... Args>
    res_errNoS ioctl(unsigned long request, Args... args);

    // man 2 fcntl
    template <typename... Args>
    res_errNoS fcntl(int cmd, Args... args);

    // see man 2 getsockopt / man 2 setsockopt / man 7 socket
    // (for options and their docs)
    res_errNoS getsockopt(
        int        level,
        int        optname,
        void      *optval,
        socklen_t *optlen
    );

    res_errNoS setsockopt(
        int         level,
        int         optname,
        const void *optval,
        socklen_t   optlen
    );

    size_errNoS read(void *buffer, size_t size);
    size_errNoS write(const void *buffer, size_t size);

    size_errNoS send(const void *buffer, size_t size, int flags);
    size_errNoS recv(void *buffer, size_t size, int flags);

    // -----------------------------------------------------
    // ^ ^ ^ function direct forwardings ^ ^ ^
    // -----------------------------------------------------
    // v v v function shortcuts and usages v v v (higher functions)
    // -----------------------------------------------------

    // suggesting use dup_suggested_options() to get options
    FDCtl_res_errNoS Dup(FDCtlInitOptions opts);

    // suggesting use dup_suggested_options() to get options
    FDCtl_res_errNoS Dup2(int newfd, FDCtlInitOptions opts);

    // note: making this function will introduce problem: this will
    //   require maintaining db and checking if result of dup2 is same as newfd
    // FDCtl_err_errNoS Dup2(std::shared_ptr<FDCtl> newfd);

    // closes current fd (if it [is set] and [not closed]) and
    // calls socket() in this object's class with same parameters
    res_errNoS Socket(
        int domain,
        int type,
        int protocol
    );

    res_errNoS Socket(
        int              domain,
        int              type,
        int              protocol,
        FDCtlInitOptions opts,
        bool             dont_close_if_open = false
    );

    err_errNoS Bind(std::shared_ptr<FDAddress> addr);

  private:
    FDAddress_err_errNoS Get_X_Name(bool sock_or_peer);

  public:
    FDAddress_err_errNoS GetSockName();
    FDAddress_err_errNoS GetPeerName();

    FDCtl_res_errNoS Connect(
        std::shared_ptr<FDAddress> addr
    );
    FDCtl_res_errNoS Connect(
        std::shared_ptr<FDAddress> addr,
        FDCtlInitOptions           opts
    );

    FDCtl_FDAddress_res_errNoS Accept();
    FDCtl_FDAddress_res_errNoS Accept(FDCtlInitOptions opts);

    // 0 on success // shortcut to getsockopt
    res_errNoS getRecvTimeout(timeval &r);

    // 0 on success  // shortcut to getsockopt
    res_errNoS getSendTimeout(timeval &s);

    // 0 on success  // shortcut to getsockopt
    res_errNoS setRecvTimeout(timeval &r);

    // 0 on success  // shortcut to getsockopt
    res_errNoS setSendTimeout(timeval &s);

    // 0 on success  // shortcut to fcntl
    res_errNoS isNonBlocking(bool &ret);

    // 0 on success // shortcut to fcntl
    res_errNoS setNonBlocking(bool blocking);

    // 0 on success
    socktype_res_errNoS getType();

    // -----------------------------------------------------
    // ^ ^ ^ function shortcuts and usages ^ ^ ^
    // -----------------------------------------------------

    res_errNoS sendFixedData(
        byte_vector              &buff,
        std::function<bool()>     make_stop,
        typeof(timespec::tv_sec)  poll_sec,
        typeof(timespec::tv_nsec) poll_nsec
    );

    res_errNoS recvFixedData(
        byte_vector              &buff,
        std::function<bool()>     make_stop,
        typeof(timespec::tv_sec)  poll_sec,
        typeof(timespec::tv_nsec) poll_nsec
    );
};

} // namespace wayround_i2p::ccutils::posix_tools

#endif
