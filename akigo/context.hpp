#ifndef WAYROUND_I2P_20240310_155748_300417
#define WAYROUND_I2P_20240310_155748_300417

#include <any>
#include <future>
#include <memory>
#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/time.hpp>

namespace wayround_i2p::akigo::context
{

class Context
{
    virtual std::tuple<wayround_i2p::akigo::time::Time, bool>
        Deadline() = 0;

    virtual std::shared_future<void>
        Done() = 0;

    virtual std::any
        Value(std::any key)
        = 0;
};

using Context_ptr = std::shared_ptr<Context>;

} // namespace wayround_i2p::akigo::context

#endif
