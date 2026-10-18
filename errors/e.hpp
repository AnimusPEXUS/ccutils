#ifndef WAYROUND_I2P_20260131_190131_5831190000
#define WAYROUND_I2P_20260131_190131_5831190000

#include <cstdint>
#include <memory>
#include <source_location>
#include <stacktrace>
#include <string>
#include <tuple>
#include <vector>

#include <wayround_i2p/ccutils/unicode/u.hpp>

// todo: enable stacktrace when gcc will support
//       it without any additional libraries

// tools for error handling

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
        UString                    text,
        const std::source_location location = std::source_location::current()
    );

    UString Error();
    UString ErrorLong();
    //    stacktrace_ptr getStacktrace();

  private:
    UString                    text;
    const std::source_location location;
    //    stacktrace_ptr st;
};

error_ptr New(
    UString                    text,
    const std::source_location location = std::source_location::current()
);

} // namespace wayround_i2p::ccutils::errors

#endif
