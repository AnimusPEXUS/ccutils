
#include <memory>

#include <wayround_i2p/ccutils/errors/e.hpp>

namespace wayround_i2p::ccutils::errors
{

BasicStringError::BasicStringError(
    UString     text,
    UString     file,
    std::size_t line
) :
    text(text),
    // st(std::make_shared<std::stacktrace>(std::stacktrace::current()))
    file(file),
    line(line)
{
}

BasicStringError::~BasicStringError()
{
}

UString BasicStringError::Error()
{
    return text;
}

UString BasicStringError::ErrorLong()
{
    return UString(
        std::format("{}:{} {}", file, line, Error())
    );
}

/*
stacktrace_ptr BasicStringError::getStacktrace()
{
    return st;
}
*/

error_ptr New(UString text)
{
    return error_ptr(dynamic_cast<error *>(new BasicStringError(text)));
}

} // namespace wayround_i2p::ccutils::errors
