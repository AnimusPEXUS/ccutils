
#include <chrono>
#include <iostream>
#include <memory>

#include <experimental/scope>

#include <wayround_i2p/ccutils/worker01/Worker01.hpp>

void worker_thr(std::function<bool()> is_stop_flag)
{
    std::experimental::scope_exit(
        []()
        {
            std::cout << "worker function exit" << std::endl;
        }
    );

    bool ticker    = false;
    int  countdown = 10;
    for (;;)
    {
        if (is_stop_flag())
        {
            std::cout << "got stop signal" << std::endl;
            break;
        }

        std::cout << "countdown: " << countdown << std::endl;
        if (countdown == 0)
        {
            std::cout << "  time to break" << std::endl;
            break;
        }

        ticker = !ticker;

        if (ticker)
        {
            std::cout << "tick" << std::endl;
        }
        else
        {
            std::cout << "tack" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        countdown--;
    }

    std::cout << "loop exit" << std::endl;
}

int main(int argc, char **args)
{

    auto stop_promise = std::shared_ptr<std::promise<void>>(new (std::promise<void>));

    wayround_i2p::ccutils::worker01::Worker01 w(&worker_thr);

    w.start(stop_promise);

    for (;;)
    {
        std::cout << "enter \"END\" to exit" << std::endl;
        std::string x;
        if (std::cin.eof())
        {
            break;
        }
        std::cin >> x;
        if (std::cin.eof())
        {
            break;
        }
        if (x == "END")
        {
            break;
        }
    }

    w.stop();

    std::cout << "waiting" << std::endl;

    stop_promise->get_future().wait();

    std::cout << "   done" << std::endl;

    return 0;
}
