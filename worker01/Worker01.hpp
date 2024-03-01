#ifndef WAYROUND_I2P_20240227_051927_170667
#define WAYROUND_I2P_20240227_051927_170667

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

class Worker01
{
  public:
    // NOTE: this is intentionnaly without ready to use std::shared_ptr creator:
    //       because, imo, most of the time you wish to use it on stack.
    //       so create via std::shared_ptr, if you need this, manually.

    // static std::shared_ptr<Worker01> create(Worker01ThreadFunction f);

    Worker01(Worker01ThreadFunction f);
    ~Worker01();

    int          start(std::shared_ptr<std::promise<void>> stop_promise);
    void         stop();
    int          restart(std::shared_ptr<std::promise<void>> stop_promise);
    WorkerStatus status();
    bool         isStopped();

  private:
    std::thread                  thr;
    const Worker01ThreadFunction threaded_function;
    void                         threaded_function_wrapper();

    WorkerStatus                        _status   = stopped;
    bool                                stop_flag = false;
    std::mutex                          start_stop_mutex;
    std::shared_ptr<std::promise<void>> stop_promise;
};

} // namespace wayround_i2p::ccutils::worker01

#endif
