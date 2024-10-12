#ifndef WAYROUND_I2P_20241007_110017_734676
#define WAYROUND_I2P_20241007_110017_734676

namespace wayround_i2p::ccutils::utils
{

/**
usage example:
```cpp
#include <wayround_i2p/ccutils/utils/concepts.hpp>

template <IsDequeOrVectorOfType<int> TypeToCheck> void test(const TypeToCheck &v) {
  for (const auto &i : v) {
    std::cout << "int " << i << std::endl;
  }

  for (std::size_t i = 0; i < v.size(); i++) {
    std::cout << "int " << v[i] << std::endl;
  }
}

template <IsDequeOrVectorOfType<std::string> TypeToCheck>
void test(const TypeToCheck &v) {
  for (const auto &i : v) {
    std::cout << "str " << i << std::endl;
  }

  for (std::size_t i = 0; i < v.size(); i++) {
    std::cout << "str " << v[i] << std::endl;
  }
}

int main(int argc, char **args) {

  test(std::deque<int>{1, 2, 3, 4, 5});
  test(std::vector<int>{1, 2, 3, 4, 5});

  test(std::vector<std::string>{"1", "2", "3", "4", "5"});

  return 0;
}
```
*/
template <class TypeToCheck, class MustBeValueType>
concept IsDequeOrVectorOfType
    = (std::same_as<std::deque<MustBeValueType>, TypeToCheck>
       || std::same_as<std::vector<MustBeValueType>, TypeToCheck>)
   && requires(TypeToCheck a, int b) {
          { a.begin() } -> std::same_as<class TypeToCheck::iterator>;
          { a.size() } -> std::integral;
          { a[b] };
      };

} // namespace wayround_i2p::ccutils::utils

#endif
