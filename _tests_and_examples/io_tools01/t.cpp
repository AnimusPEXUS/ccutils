
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <thread>

#include <experimental/scope>

#include <wayround_i2p/ccutils/io_tools/utils.hpp>

#include <sys/socket.h>
#include <sys/un.h>

const char *server_addr = "/tmp/testservaddr";

int main_server()
{
    int err = 0;

    sockaddr_un own_addr = {0};
    int         sock_fd  = 0;

    own_addr.sun_family = AF_UNIX;

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        return -1;
    }
    auto x01 = std::experimental::scope_exit(
        [&]()
        { close(sock_fd); }
    );

    std::strcpy(own_addr.sun_path, server_addr);

    err = bind(sock_fd, (sockaddr *)(&own_addr), sizeof(own_addr));
    if (err != 0)
    {
        return 1;
    }

    for (;;)
    {
    }

    return 0;
}

int main_client()
{
    int err = 0;

    sockaddr_un rmt_addr = {0};
    int         sock_fd  = 0;

    rmt_addr.sun_family = AF_UNIX;

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        return -1;
    }
    auto x01 = std::experimental::scope_exit(
        [&]()
        { close(sock_fd); }
    );

    std::strcpy(rmt_addr.sun_path, server_addr);

    err = connect(sock_fd, (sockaddr *)(&rmt_addr), sizeof(rmt_addr));
    if (err != 0)
    {
        return err;
    }

    for (;;)
    {
    }

    return 0;
}

int main(int argc, char **args)
{
    std::thread th1(&main_server);
    std::thread th2(&main_client);

    th1.join();
    th2.join();

    return 0;
}
