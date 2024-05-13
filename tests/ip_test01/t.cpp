
#include <iostream>

#include <experimental/scope>

#include <wayround_i2p/ccutils/ip/ip.hpp>

int main(int argc, char **args)
{
    bool         tests_ok     = false;
    unsigned int errors_count = 0;

    {
        auto ip = wayround_i2p::ccutils::ip::IP::create();
        if (!ip)
        {
            std::cout << "error: can't create IP obj. line: "
                      << __LINE__ << std::endl;
            errors_count++;
            goto end;
        }

        {
            auto s     = ip->toString();
            auto s_err = std::get<1>(s);
            if (!s_err)
            {
                std::cout << "FAIL: this case should return error"
                          << std::endl;
                errors_count++;
                goto end;
            }

            auto s_err_s = s_err->Error();

            if (s_err_s != "not set")
            {
                std::cout << "FAIL: this case should return 'not set' error."
                          << " returned err is: " << s_err_s << std::endl;
                errors_count++;
                goto end;
            }
        }

    end:
    }

    tests_ok = errors_count == 0;

    if (tests_ok)
    {
        std::cout << "tests passed." << std::endl;
        return 0;
    }
    else
    {
        return 1;
    }
}
