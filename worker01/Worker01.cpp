
#include <wayround_i2p/ccutils/worker01/Worker01.hpp>

namespace wayround_i2p::ccutils::worker01
{

Worker01::Worker01() :
    _status(stopped)
{
}

Worker01::Worker01(Worker01ThreadFunction f) :
    Worker01()
{
    setFunction(f);
}

Worker01::~Worker01()
{
}

void Worker01::setFunction(Worker01ThreadFunction f)
{
    threaded_function = f;
}

int Worker01::start()
{
    return start(nullptr);
}

int Worker01::start(std::shared_ptr<std::promise<void>> stop_promise)
{
    start_stop_mutex.lock();
    auto se01 = std::experimental::scope_exit(
        [&]()
        {
            start_stop_mutex.unlock();
        }
    );

    if (!threaded_function)
    {
        return 2;
    }

    if (!isStopped())
    {
        return 1;
    }

    this->stop_promise = stop_promise;
    _status            = starting;
    stop_flag          = false;
    thr                = std::thread(
        &Worker01::threaded_function_wrapper,
        this
    );
    thr.detach();

    return 0;
}

void Worker01::stop()
{
    start_stop_mutex.lock();
    auto se01 = std::experimental::scope_exit(
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
    return restart(nullptr);
}

int Worker01::restart(std::shared_ptr<std::promise<void>> stop_promise)
{
    int err = 0;

    stop();

    if (this->stop_promise)
    {
        this->stop_promise->get_future().wait();
    }

    err = start(stop_promise);
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

void Worker01::threaded_function_wrapper()
{
    auto se01 = std::experimental::scope_exit(
        [&]()
        {
            _status = stopped;
            if (stop_promise)
            {
                stop_promise->set_value();
            }
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
