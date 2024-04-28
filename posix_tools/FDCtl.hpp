#ifndef WAYROUND_I2P_20240408_004458_731521
#define WAYROUND_I2P_20240408_004458_731521

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
    std::shared_ptr<FDCtl> fd;
};

struct FDAddress_err_errNoS : err_errNoS
{
    std::shared_ptr<FDCtl> fd;
};

struct FDCtlInitOptions
{
    int  fd               = 0;
    bool is_closed        = true;
    bool close_on_destroy = false;
};

class FDCtl
{
  private:
    FDCtlInitOptions opts;

    std::weak_ptr<FDCtl> own_ptr;

  protected:
    FDCtl(FDCtlInitOptions opts);

  public:
    static std::shared_ptr<FDCtl> create(FDCtlInitOptions opts);

    ~FDCtl();

    // does direct call without any checks and/or precautions.
    // in any case this puts FDCtl into closed state.
    err_errNoS close();

    // does internal checks and if everything's ok - calls this->close().
    // if already closed - this is not error.
    err_errNoS Close();

    int getFD();

    bool GetCloseOnDestroy();
    void SetCloseOnDestroy(bool);

    // gets address data from fd in this FDCtl and puts it into addr
    err_errNoS setFDAddress(std::shared_ptr<FDAddress> addr);

    // v v v function direct forwardings v v v

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
    err_errNoS connect(struct sockaddr *addr, socklen_t length);
    err_errNoS listen(int n);
    err_errNoS accept(struct sockaddr *addr, socklen_t *length_ptr);

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

    // ^ ^ ^ function direct forwardings ^ ^ ^

    // v v v function shortcuts and usages v v v

    FDCtl_res_errNoS Dup(bool close_on_destroy = false);

    FDCtl_res_errNoS Dup2(int newfd, bool close_on_destroy = false);

    // note: making this function will introduce problem: this will
    //   require maintaining db and checking if result of dup2 is same as newfd
    // FDCtl_err_errNoS Dup2(std::shared_ptr<FDCtl> newfd);

    // closes current fd (if it [is set] and [not closed]) and
    // calls socket() in this object's class with same parameters
    res_errNoS Socket(int domain, int type, int protocol);

    err_errNoS Bind(std::shared_ptr<FDAddress> addr);

    FDAddress_err_errNoS GetSockName();

    err_errNoS Connect(std::shared_ptr<FDAddress> addr);

    FDAddress_err_errNoS Accept();

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
    res_errNoS getType(int &type);

    // ^ ^ ^ function shortcuts and usages ^ ^ ^

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
