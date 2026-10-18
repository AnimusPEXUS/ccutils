
#include <memory>

#include <wayround_i2p/ccutils/errors/e.hpp>

namespace wayround_i2p::ccutils::errors
{

BasicStringError::BasicStringError(
    UString                    text,
    const std::source_location location
)
    : text(text), location(location)
{
}

UString BasicStringError::Error()
{
    return UString(
        std::format(
            "{}:{} :: {}",
            location.file_name(),
            location.line(),
            text
        )
    );
}

/*
stacktrace_ptr BasicStringError::getStacktrace()
{
    return st;
}
*/

error_ptr New(
    UString                    text,
    const std::source_location location
)
{
    return error_ptr(dynamic_cast<error *>(new BasicStringError(text, location)));
}

} // namespace wayround_i2p::ccutils::errors
