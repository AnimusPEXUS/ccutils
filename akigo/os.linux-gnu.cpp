
#include <wayround_i2p/ccutils/akigo/errors.hpp>
#include <wayround_i2p/ccutils/akigo/os.hpp>

#include <stdio.h>

namespace wayround_i2p::akigo::os
{

using error_ptr = wayround_i2p::akigo::builtin::error_ptr;

error_ptr Remove(ustring name)
{

    int int_err = remove(name.string_utf8().c_str());

    if (int_err != 0)
    {
        return wayround_i2p::akigo::errors::New(
            ustring("couldn't remove file: ") + name
        );
    }

    return nullptr;
}

} // namespace wayround_i2p::akigo::os
