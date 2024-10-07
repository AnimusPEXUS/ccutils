#ifndef WAYROUND_I2P_20241007_110017_734676
#define WAYROUND_I2P_20241007_110017_734676

namespace wayround_i2p::ccutils::utils
{

/**
usage example:
```cpp
#include <wayround_i2p/ccutils/utils/concepts.hpp>

template <wayround_i2p::ccutils::utils::IsBeginnableSizableIndexaccessable T> void test(const T &v) {
  for (const auto &i : v) {
    std::cout << i << std::endl;
  }

  for (std::size_t i = 0; i < v.size(); i++) {
    std::cout << v[i] << std::endl;
  }
}

int main(int argc, char **args) {

  test(std::deque<int>{1, 2, 3, 4, 5});
  test(std::vector<int>{1, 2, 3, 4, 5});

  return 0;
}
```
*/
template <class T>
concept IsBeginnableSizableIndexaccessable = requires(T a, int b) {
    { a.begin() } -> std::same_as<class T::iterator>;
    { a.size() } -> std::integral;
    { a[b] };
};

} // namespace wayround_i2p::ccutils::utils

#endif
