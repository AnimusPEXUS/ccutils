#ifndef WAYROUND_I2P_20240718_170059_659019
#define WAYROUND_I2P_20240718_170059_659019

// #include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::unicode
{
class UString;
}

namespace wayround_i2p::ccutils::repr
{

// template <typename T>
// concept bool HasFunc1 =
//   requires(T t) {
//       { t.func1() } -> int;
//   };

// template <typename T>
// concept RepresentableAsText = requires(T t) {
//     { t.repr_as_text() } -> std::same_as<wayround_i2p::ccutils::unicode::UString>;
// };

// template <typename T>
//     requires RepresentableAsText<T>
// wayround_i2p::ccutils::unicode::UString repr(T t)
// {
//     return t.repr_as_text();
// }

class RepresentableAsText
{
  public:
    virtual wayround_i2p::ccutils::unicode::UString repr_as_text() = 0;
};

} // namespace wayround_i2p::ccutils::repr

#endif
