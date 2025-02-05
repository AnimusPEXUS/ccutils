#ifndef WAYROUND_I2P_20250205_113427_132020
#define WAYROUND_I2P_20250205_113427_132020

#include <cstdint>
#include <memory>
#include <stacktrace>
#include <string>
#include <tuple>
#include <vector>

#include <wayround_i2p/ccutils/unicode/u.hpp>

// todo: make stacktrace when gcc will support
//       it without any additional libraries

namespace wayround_i2p::ccutils::errors
{

using UString = wayround_i2p::ccutils::unicode::UString;

// using stacktrace_ptr = decltype(std::make_shared<std::stacktrace>(std::stacktrace::current()));

class error : std::exception
{
  public:
    virtual UString Error() = 0;
};

using error_ptr = std::shared_ptr<error>;

class BasicStringError : public error
{
  public:
    BasicStringError(
        UString     text,
        UString     file = __FILE__,
        std::size_t line = __LINE__
    );

    ~BasicStringError();

    UString Error();
    UString ErrorLong();
    //    stacktrace_ptr getStacktrace();

  private:
    UString     text;
    UString     file;
    std::size_t line;
    //    stacktrace_ptr st;
};

error_ptr New(UString text, UString file, std::size_t line);

} // namespace wayround_i2p::ccutils::errors

#endif
