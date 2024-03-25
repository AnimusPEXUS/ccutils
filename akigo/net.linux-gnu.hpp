#ifndef WAYROUND_I2P_20240323_153956_24586a123
#define WAYROUND_I2P_20240323_153956_24586a123

#include <wayround_i2p/ccutils/akigo/net.hpp>

namespace wayround_i2p::akigo::net
{

class StdSocketConn : wayround_i2p::akigo::net::Conn
{
  private:
    int active_fd = 0;
	
public:
	
};

} // namespace wayround_i2p::akigo::net

#endif
