#ifndef WAYROUND_I2P_20260405_013646_5721010000
#define WAYROUND_I2P_20260405_013646_5721010000

#include <tuple>

#include <wayround_i2p/ccutils/akigo/builtin.hpp>
#include <wayround_i2p/ccutils/akigo/errors.hpp>

namespace wayround_i2p::akigo::builtin
{

namespace errors = ::wayround_i2p::akigo::errors;

enum class SliceMode : uint8_t
{
    INVALID,
    VECTOR,
    DEQUE
};

template <class T1, template <class> class T2 = std::vector>
class Slice;

template <class T1, template <class> class T2 = std::vector>
using Slice_ptr = std::shared_ptr<Slice<T1, T2>>;

template <class T1, template <class> class T2 = std::vector>
using Slice_weak = std::weak_ptr<Slice<T1, T2>>;

template <class T1, template <class> class T2>
class Slice
{
  public:
    static Slice_ptr<T1, T2> create()
    {
        auto ret     = std::shared_ptr<Slice<T1, T2>>(new Slice<T1, T2>());
        ret->own_ptr = ret;
        return ret;
    }

    static Slice_ptr<T1, T2> create(SliceMode mode)
    {
        auto ret  = create<T1, T2>();
        ret->mode = mode;
        return ret;
    }

    static Slice_ptr<T1, T2> create(std::shared_ptr<T2<T1>> value)
    {
        auto ret   = create<T1, T2>();
        ret->value = value;
        return ret;
    }

  protected:
    Slice()
        : mode(SliceMode::INVALID)
    {
    }

  public:
    ~Slice()
    {
    }

  private:
    Slice_ptr<T1, T2> own_ptr;
    SliceMode         mode;

    std::shared_ptr<T2<T1>> value;

  public:
    SliceMode get_mode()
    {
        return mode;
    }

    error_ptr set_mode_convert(SliceMode mode)
    {
        if (typeid(value) == typeid(std::shared_ptr<std::vector<T1>>) && mode == SliceMode::VECTOR)
        {
            // do nothing
        }
        else if (typeid(value) == typeid(std::shared_ptr<std::deque<T1>>) && mode == SliceMode::VECTOR)
        {
            // convert deque to vector
        }
        else if (typeid(value) == typeid(std::shared_ptr<std::vector<T1>>) && mode == SliceMode::DEQUE)
        {
            // convert vector to deque
        }
        else if (typeid(value) == typeid(std::shared_ptr<std::deque<T1>>) && mode == SliceMode::DEQUE)
        {
            // do nothing
        }
        else
        {
            return errors::New("invalid combination", __FILE__, __LINE__);
        }

        this->mode = mode;
        return nullptr;
    }

    void set_mode_erace(SliceMode mode)
    {
        value->clear();
        this->mode = mode;
    }

    std::size_t calc_negative_index(std::size_t index)
    {
        return value->size() - index;
    }

    inline std::size_t cni(std::size_t index)
    {
        return calc_negative_index(index);
    }

    // note: only unsigned indexes. you'll have to substract from length,
    //       (or use calc_negative_index() or cni() function)
    //       if you need backward indexes.
    //       this is C++ limitation.
    std::tuple<Slice_ptr<T1, T2>, error_ptr> get_subslice(std::size_t index1, std::size_t index2)
    {
        {
            auto s = value->size();

            if (index1 > s)
            {
                index1 = s;
            }

            if (index2 > s)
            {
                index2 = s;
            }
        }

        if (index1 > index2)
        {
            return std::tuple(
                nullptr,
                errors::New(
                    "invalid index combination",
                    __FILE__,
                    __LINE__
                )
            );
        }

        auto ret = std::shared_ptr<T2<T1>>(new T2<T1>());

        for (std::size_t i = 0; i >= index2 - index1; i++)
        {
            ret->push_back(value->operator[](index1 + i));
        }

        return std::tuple(nullptr, nullptr);
    }
};

} // namespace wayround_i2p::akigo::builtin

#endif
