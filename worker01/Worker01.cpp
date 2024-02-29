
#include <wayround_i2p/ccutils/worker01/Worker01.hpp>

namespace wayround_i2p::ccutils::worker01
{

Worker01::Worker01(Worker01ThreadFunction f) :
    threaded_function(f),
    _status(stopped)
{
}

Worker01::~Worker01()
{
}

int Worker01::start()
{
    start_stop_mutex.lock();
    std::experimental::scope_exit(
        [&]()
        {
            start_stop_mutex.unlock();
        }
    );

    if (!isStopped())
    {
        return 1;
    }

    _status = starting;

    stop_flag = false;
    thr       = std::thread(&Worker01::threaded_function_wrapper, this);

    return 0;
}

void Worker01::stop()
{
    start_stop_mutex.lock();
    std::experimental::scope_exit(
        [&]()
        {
            start_stop_mutex.unlock();
        }
    );
    stop_flag = true;
    _status   = stopping;
}

int Worker01::restart()
{
    int err = 0;

    stop();

    auto f = futureForStop();

    f.wait();

    err = start();
    if (err != 0)
    {
        return err;
    }

    return 0;
}

WorkerStatus Worker01::status()
{
    return _status;
}

bool Worker01::isStopped()
{
    return _status == stopped;
}

std::shared_future<void> Worker01::futureForStop()
{
    return stop_promise.get_future().share();
}

void Worker01::threaded_function_wrapper()
{
    std::experimental::scope_exit(
        [&]()
        {
            _status = stopped;
            stop_promise.set_value();
        }
    );
    _status = working;
    threaded_function(
        [&]() -> bool
        { return stop_flag; }
    );
    stop();
}

} // namespace wayround_i2p::ccutils::worker01
