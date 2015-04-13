#ifndef KYRINBOX_SRC_COMMON_CRYPTO_KYRIN_HEX_H_
#define KYRINBOX_SRC_COMMON_CRYPTO_KYRIN_HEX_H_

namespace kyrin {
namespace common {
namespace crypto {

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

}
}
} /* kyrin */

#endif /* end of include guard: KYRINBOX_SRC_COMMON_CRYPTO_KYRIN_HEX_H_ */
