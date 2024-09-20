#ifndef WAYROUND_I2P_20240920_174507_394643
#define WAYROUND_I2P_20240920_174507_394643

// #include <wayround_i2p/ccutils/unicode/u.hpp>

namespace wayround_i2p::ccutils::unicode
{
class UString;
}

namespace wayround_i2p::ccutils::repr
{

class RepresentableAsText
{
  public:
    virtual wayround_i2p::ccutils::unicode::UString repr_as_text() const = 0;
};

} // namespace wayround_i2p::ccutils::repr

#endif
