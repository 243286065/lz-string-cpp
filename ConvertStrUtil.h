#ifndef LZ_STRING_CONVERT_STR_UTIL_H_
#define LZ_STRING_CONVERT_STR_UTIL_H_

#include <string>

namespace lzstring
{
extern std::string to_string(std::wstring const &src);

// 把一个string转化为wstring
extern std::wstring to_wstring(std::string const &src);

} // namespace lzstring

#endif // LZ_STRING_CONVERT_STR_UTIL_H_