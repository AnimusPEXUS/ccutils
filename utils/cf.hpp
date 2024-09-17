#include <chrono>
#include <functional>

// todo: testing required

namespace wayround_i2p::ccutils::utils::cached_function
{

template <class R, class... Args>
class CachedFunction
{
  public:
    CachedFunction(
        std::function<bool(Args... args)>                 updateRequired,
        std::function<void(R &cached_data, Args... args)> update
    ) :
        updateRequired(updateRequired),
        update(update)
    {
    }

    ~CachedFunction()
    {
    }

    R getCaching(Args... args)
    {
        if (updateRequired(args...))
        {
            update(cached_data, args...);
        }

        return cached_data;
    }

    std::function<bool(Args... args)>                 updateRequired;
    std::function<void(R &cached_data, Args... args)> update;

    R cached_data;
};

template <class R, class... Args>
class SteadyClockCachedFunction : public CachedFunction<R, Args...>
{
  public:
    SteadyClockCachedFunction(
        std::chrono::time_point<std::chrono::steady_clock>
            &tracked_object_last_change_time_point,

        std::function<void(R &cached_data, Args... args)> update
    ) :
        CachedFunction<R, Args...>(
            [&](Args... args)
            {
                return !(
                    tracked_object_last_change_time_point < last_time_function_result_calculated
                );
            },
            [&](R &cached_data, Args... args)
            {
                update(cached_data, args...);
                tracked_object_last_change_time_point = std::chrono::steady_clock::now();
            }
        )
    {
        last_time_function_result_calculated
            = tracked_object_last_change_time_point;
    }

    ~SteadyClockCachedFunction()
    {
    }

  private:
    std::chrono::time_point<std::chrono::steady_clock>
        last_time_function_result_calculated;
};

} // namespace wayround_i2p::ccutils::utils::cached_function
