#ifndef WAYROUND_I2P_20260131_190131_5755760000
#define WAYROUND_I2P_20260131_190131_5755760000

#include <functional>
#include <future>
#include <memory>
#include <mutex>

#include <experimental/scope>

namespace wayround_i2p::ccutils::worker01
{

enum WorkerStatus : unsigned char
{
    stopped,
    starting,
    working,
    stopping
};

using Worker01ThreadFunction = std::function<void(std::function<bool()>)>;

class Worker01I
{
  public:
    virtual int          start(std::shared_ptr<std::promise<void>> stop_promise)   = 0;
    virtual void         stop()                                                    = 0;
    virtual int          restart(std::shared_ptr<std::promise<void>> stop_promise) = 0;
    virtual WorkerStatus status()                                                  = 0;
    virtual bool         isStopped()                                               = 0;
};

class Worker01 : public Worker01I
{
  public:
    // NOTE: this is intentionnaly without ready to use std::shared_ptr creator:
    //       because, imo, most of the time you wish to use it on stack.
    //       so create via std::shared_ptr, if you need this, manually.

    // static std::shared_ptr<Worker01> create(Worker01ThreadFunction f);

    Worker01();
    Worker01(Worker01ThreadFunction f);
    ~Worker01();

    void setFunction(Worker01ThreadFunction f);

    int          start();
    int          start(std::shared_ptr<std::promise<void>> stop_promise);
    void         stop();
    int          restart();
    int          restart(std::shared_ptr<std::promise<void>> stop_promise);
    WorkerStatus status();
    bool         isStopped();

  private:
    std::thread            thr;
    Worker01ThreadFunction threaded_function;
    void                   threaded_function_wrapper();

    WorkerStatus                        _status   = stopped;
    bool                                stop_flag = false;
    std::mutex                          start_stop_mutex;
    std::shared_ptr<std::promise<void>> stop_promise;
};

} // namespace wayround_i2p::ccutils::worker01

#endif
