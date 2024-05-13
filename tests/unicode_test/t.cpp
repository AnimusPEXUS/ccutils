
#include <iostream>

#include <experimental/scope>

#include <wayround_i2p/ccutils/unicode/u.hpp>

int main(int argc, char **args)
{

    std::cout << "test std::string:" << std::string("test") << std::endl;
    std::cout << "test UString:" << wayround_i2p::ccutils::unicode::UString("test").string_utf8() << std::endl;
    std::cout << "test UString2:" << wayround_i2p::ccutils::unicode::UString("test") << std::endl;

    std::cout << "тест length (must be 4): " << wayround_i2p::ccutils::unicode::UString("тест").length() << std::endl;

    return 0;
}
