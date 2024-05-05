
#include <wayround_i2p/ccutils/errors/e.hpp>

namespace wayround_i2p::ccutils::errors
{

BasicStringError::BasicStringError(UString text) :
    text(text)
{
}

BasicStringError::~BasicStringError()
{
}

UString BasicStringError::Error()
{
    return text;
}

error_ptr New(UString text)
{
    return error_ptr(dynamic_cast<error *>(new BasicStringError(text)));
}

} // namespace wayround_i2p::ccutils::errors
