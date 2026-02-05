#ifndef WAYROUND_I2P_20260131_190131_5817270000
#define WAYROUND_I2P_20260131_190131_5817270000

// tf01 - types and functions resamblance #01
// see examples at bottom

#include <initializer_list>

#include <deque>
#include <vector>

#include <string>

#include <iostream>
#include <memory>

template <class T>
using go_slice_v = std::shared_ptr<std::vector<T>>;

template <class T>
using go_slice_d = std::shared_ptr<std::deque<T>>;

template <class TypeToCheck, class MustBeValueType>
concept go_IsSliceVOrSliceDOfType
    = (std::same_as<go_slice_v<MustBeValueType>, TypeToCheck>
       || std::same_as<go_slice_d<MustBeValueType>, TypeToCheck>);

template <class T, go_IsSliceVOrSliceDOfType<T> Tr>
Tr go_append(Tr s, T v)
{
    auto ret = Tr(new decltype(s)::element_type());
    for (typename decltype(s)::element_type::size_type i = 0; i < s->size(); i++)
    {
        ret->push_back(s->at(i));
    }
    ret->push_back(v);
    return ret;
}

template <class T, go_IsSliceVOrSliceDOfType<T> Tv, go_IsSliceVOrSliceDOfType<T> Tr>
Tr go_append(Tr s, Tv v)
{
    auto ret = Tr(new decltype(s)::element_type());
    for (typename decltype(s)::element_type::size_type i = 0; i < s->size(); i++)
    {
        ret->push_back(s->at(i));
    }
    for (typename decltype(v)::element_type::size_type i = 0; i < v->size(); i++)
    {
        ret->push_back(v->at(i));
    }
    return ret;
}

template <class... Args>
using go_il = std::initializer_list<Args...>;

template <class T, class... Args>
T go_make(Args... args)
{
    auto ret = T(new T::element_type(args...));
    return ret;
}

template <class T1, class T2>
T1 go_make(go_il<T2> init)
{
    auto ret = T1(new T1::element_type(init));
    return ret;
}

/*

int main(int argc, char **args)
{
    auto g = go_make<go_slice_v<int>>();

    auto x1 = go_make<go_slice_v<int>>(go_il({3, 4, 5}));
    auto x2 = go_make<go_slice_v<int>>({6, 7, 8});

    auto res = go_append(g, 1);
    res      = go_append(res, 2);
    res      = go_append<int>(res, x1);
    res      = go_append<int>(res, x2);

    for (int i = 0; i < res->size(); i++)
    {
        std::cout << i << " : " << res->at(i) << std::endl;
    }

    std::cout << "type: " << typeid(go_il<int>({3, 4, 5})).name() << std::endl;
    std::cout << "type: " << typeid(go_il({3, 4, 5})).name() << std::endl;

    return 0;
}


*/

#endif
