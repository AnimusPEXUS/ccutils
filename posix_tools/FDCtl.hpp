#ifndef WAYROUND_I2P_20250205_113427_110065
#define WAYROUND_I2P_20250205_113427_110065

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

using FDCtl_ptr  = std::shared_ptr<FDCtl>;
using FDCtl_weak = std::weak_ptr<FDCtl>;

// class FDAddress;
// int FDAddress::setAddrBuff(std::vector<std::uint8_t> addr_buff);

// result structures naming
//
//   ends with S indicating it's a struct
//   res   - for function result returned with return keyword;
//           res is for functions, which doesn't use return to tell
//           about errors.
//   err   - unlike ret, err is for functions, which use return to
//           rell about errors
//   errNo - errno value. unix/posix functions sets special variable
//             named 'errno' to provide additional error info
//   size  - some functions return size
//
//  error results:
//    you can quick-check with not_ok() function if value is not ok.
//    for 'res'  functions -1 usually means error;
//    for 'err'  functions  0 usually means ok, and other
//                  values means errors or some conditions;
//    for 'size' functions -1 usually means error;

struct errS
{
    int  err = -1; // real func result
    bool not_ok()
    {
        return err != 0;
    }

    bool is_ok()
    {
        return !not_ok();
    }
};

struct resS
{
    int  res = -1; // real func result
    bool not_ok()
    {
        return res == -1;
    }

    bool is_ok()
    {
        return !not_ok();
    }
};

struct sizeS
{
    int  size = -1; // real func result
    bool not_ok()
    {
        return size == -1;
    }

    bool is_ok()
    {
        return !not_ok();
    }
};

struct errNoS
{
    int errNo = 1; // errno on func error
};

struct err_errNoS
    : errS
    , errNoS
{
};

struct res_errNoS
    : resS
    , errNoS
{
};

struct size_errNoS
    : sizeS
    , errNoS
{
};

struct FDCtl_res_errNoS
    : res_errNoS
{
    FDCtl_ptr fdctl;
};

struct FDAddress_err_errNoS
    : err_errNoS
{
    FDAddress_ptr addr;
};

struct FDCtl_FDAddress_res_errNoS
    : res_errNoS
{
    FDCtl_ptr     fdctl;
    FDAddress_ptr addr;
};

struct intval_res_errNoS
    : res_errNoS
{
    int intval;
};

struct domain_type_protocol_res_errNoS
    : res_errNoS
{
    int domain;
    int type;
    int protocol;
};

struct FDCtlInitOptions
{
    bool is_open          = false;
    bool close_on_destroy = false;

    // todo: make funcs use those
    bool guard_lower_functions_from_running_if_fdctl_in_closed_state  = true;
    bool guard_higher_functions_from_running_if_fdctl_in_closed_state = true;
};

consteval FDCtlInitOptions fdctl_normal_closed_options();
consteval FDCtlInitOptions fdctl_normal_open_options();

class FDCtl
{
    // todo: add file-related functions

    // todo: make more handy create() and constructor functions,
    //       to shortcut FDCtlInitOptions opts
    // todo: enough?

  public:
    // creates new FDCtl. if you have no predefined fd - it's safe to pass 0 as fd.
    // internal fd value will be replaced by calling some functions below.
    // for example select/Select functions replaces FdCtl instance's fd value.
    static FDCtl_ptr create();
    static FDCtl_ptr create(int fd);
    static FDCtl_ptr create(FDCtlInitOptions opts);
    static FDCtl_ptr create(int fd, FDCtlInitOptions opts);

  protected:
    FDCtl(int fd, FDCtlInitOptions opts);

  public:
    ~FDCtl();

  private:
    FDCtl_weak own_ptr;

  public:
    int              fd = 0;
    FDCtlInitOptions opts;

    // does direct call without any checks and/or precautions.
    // in any case this puts FDCtl into closed state.
    err_errNoS close();

    // does internal checks and if everything's ok - calls this->close().
    // if already closed - this is not error.
    err_errNoS Close();

    // this doesn't performs any checks and simply replaces contained fd and options
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
    err_errNoS listen(int n); // todo: something wise needs to be done about this 'n' parameter
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
    // FDCtl_err_errNoS Dup2(FDCtl_ptr newfd);

    // Closes current fd (if it [is set] and [not closed]) and
    // calls socket() in this object's class with same parameters.
    // Doesn't preserve copy of domain/type/protocol values.
    // use getDomainTypeProtocol() to get back those values form actual socket itself.
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

    err_errNoS Bind(FDAddress_ptr addr);

  private:
    FDAddress_err_errNoS Get_X_Name(bool sock_or_peer);

  public:
    FDAddress_err_errNoS GetSockName();
    FDAddress_err_errNoS GetPeerName();

    FDCtl_res_errNoS Connect(
        FDAddress_ptr addr
    );
    FDCtl_res_errNoS Connect(
        FDAddress_ptr    addr,
        FDCtlInitOptions opts
    );

    err_errNoS Listen();
    err_errNoS Listen(int n); // todo: something wise needs to be done about this 'n' parameter

    FDCtl_FDAddress_res_errNoS Accept();
    FDCtl_FDAddress_res_errNoS Accept(FDCtlInitOptions opts);

    // res==0 on success // shortcut to getsockopt
    res_errNoS getRecvTimeout(timeval &r);

    // res==0 on success  // shortcut to getsockopt
    res_errNoS getSendTimeout(timeval &s);

    // res==0 on success  // shortcut to getsockopt
    res_errNoS setRecvTimeout(timeval &r);

    // res==0 on success  // shortcut to getsockopt
    res_errNoS setSendTimeout(timeval &s);

    // res==0 on success  // shortcut to fcntl
    res_errNoS isNonBlocking(bool &ret);

    // res==0 on success // shortcut to fcntl
    res_errNoS setNonBlocking(bool blocking);

    // res==0 on success
    intval_res_errNoS getDomain();

    // res==0 on success
    intval_res_errNoS getType();

    // res==0 on success
    intval_res_errNoS getProtocol();

    // internally calls getDomain/getType/getProtocol and returns
    // combined result or one of errors
    domain_type_protocol_res_errNoS getDomainTypeProtocol();

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
