
#include <cstring>
#include <iostream>

#include <experimental/scope>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/net.hpp>
#include <wayround_i2p/ccutils/akigo/os.hpp>

namespace akigo_ns = wayround_i2p::akigo;

std::tuple<akigo_ns::builtin::byte_slice, bool> TryReadLine(
    akigo_ns::builtin::byte_slice buff
)
{

    akigo_ns::builtin::byte_slice ret_ret;
    bool                          ret_ok;

    std::cout << "searching new line" << std::endl;

    std::experimental::scope_exit(
        [&ret_ok]()
        {
            if (!ret_ok)
            {
                std::cout << "not found" << std::endl;
            }
            else
            {
                std::cout << "found" << std::endl;
            }
        }
    );

    auto buff_p = *buff;

    for (decltype(buff_p)::size_type i = 0; i != buff_p.size(); i++)
    {
        auto v = buff_p.at(i);
        if (v == '\n')
        {
            return std::tuple(
                akigo_ns::builtin::byte_slice(
                    buff_p.begin(),
                    buff_p.begin() + i
                ),      
                true
            );
        }
    }

    return std::tuple(ret_ret, ret_ok);
}

void ProcessLine(
    akigo_ns::builtin::byte_slice line
)
{
    std::string text;
    text.resize(line.size());
    std::memcpy(text.data(), line.data(), line.size());
    std::cout << text << std::endl;
}

void wkr(akigo_ns::net::Conn_ptr c)
{
    std::experimental::scope_exit(
        []()
        {
            std::cout << "wkr exit" << std::endl;
        }
    );

    akigo_ns::builtin::byte_slice buff;

    while (true)
    {
    }
}

int main(int argc, char **args)
{
    akigo_ns::net::UnixAddr_ptr uaddr
        = akigo_ns::net::UnixAddr_ptr(new (akigo_ns::net::UnixAddr));

    uaddr->Net  = "unix";
    uaddr->Name = "./test_unix_socket";

    akigo_ns::os::Remove("./test_unix_socket");

    akigo_ns::net::UnixListener_ptr s;
    akigo_ns::builtin::error_ptr    err;

    std::tie(s, err) = akigo_ns::net::ListenUnix("unix", uaddr);
    if (err)
    {
        std::cout << "ListenUnix err: " << err << std::endl;
        return 1;
    }

    if (!s)
    {
        std::cout << "s is null" << std::endl;
        return 1;
    }

    for (;;)
    {
        akigo_ns::net::Conn_ptr c;
        std::tie(c, err) = s->Accept();
        if (err)
        {
            std::cout << "Accept err: " << err << std::endl;
            return 1;
        }

        std::thread thr(
            [&c]()
            {
                akigo_ns::net::Conn_ptr c2 = std::move(c);
                wkr(c2);
            }
        );
    }

    return 0;
}
