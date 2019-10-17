#ifndef LZW_CONVERT_STR_UTIL_H_
#define LZW_CONVERT_STR_UTIL_H_

#include <string>

namespace lzw
{
extern std::string to_string(std::wstring const &src);

// 把一个string转化为wstring
extern std::wstring to_wstring(std::string const &src);

} // namespace lzw

#endif // LZW_CONVERT_STR_UTIL_H_