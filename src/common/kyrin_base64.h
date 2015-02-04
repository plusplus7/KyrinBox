#include <string>

namespace kyrin {
namespace common {
namespace crypto {

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

} /* crypto */
} /* common */
} /* kyrin */
