#include <chrono>
#include <functional>

// todo: testing required

namespace wayround_i2p::ccutils::utils::catched_function
{

template <class R, class... Args>
class CatchedFunction
{
  public:
    CatchedFunction(
        std::function<bool(Args... args)>                  updateRequired,
        std::function<void(R &catched_data, Args... args)> update
    ) :
        updateRequired(updateRequired),
        update(update)
    {
    }

    ~CatchedFunction()
    {
    }

    R operator()(Args... args)
    {
        if (updateRequired(args...))
        {
            update(&catched_data, args...);
        }

        return catched_data;
    }

    std::function<bool(Args... args)>                  updateRequired;
    std::function<void(R &catched_data, Args... args)> update;

    R catched_data;
};

template <class R, class... Args>
class SteadyClockCatchedFunction : CatchedFunction<R, Args...>
{
  public:
    SteadyClockCatchedFunction(
        std::chrono::time_point<std::chrono::steady_clock>
            &tracked_object_last_change_time_point,

        std::function<void(R &catched_data, Args... args)> update
    ) :
        CatchedFunction<R, Args...>(
            [&](Args... args)
            {
                return !(
                    tracked_object_last_change_time_point < last_time_function_result_calculated
                );
            },
            [&](R &catched_data, Args... args)
            {
                update(catched_data, args...);
                tracked_object_last_change_time_point = std::chrono::steady_clock::now();
            }
        )
    {
        last_time_function_result_calculated
            = tracked_object_last_change_time_point;
    }

    ~SteadyClockCatchedFunction()
    {
    }

  private:
    std::chrono::time_point<std::chrono::steady_clock>
        last_time_function_result_calculated;
};

} // namespace wayround_i2p::ccutils::utils::catched_function
