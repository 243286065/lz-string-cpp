#include "ConvertStrUtil.h"

#include <codecvt>
#include <cstdlib>
#include <clocale>
#include <vector>

namespace lzstring
{
std::string to_string(std::wstring const &src)
{
    std::setlocale(LC_CTYPE, "");

    std::string dest;
    size_t const mbs_len = wcstombs(NULL, src.c_str(), 0);
    std::vector<char> tmp(mbs_len + 1);
    wcstombs(&tmp[0], src.c_str(), tmp.size());

    dest.assign(tmp.begin(), tmp.end() - 1);

    return dest;
}

// 把一个string转化为wstring
std::wstring to_wstring(std::string const &src)
{
    //   std::setlocale(LC_CTYPE, "");
    std::setlocale(LC_CTYPE, "zh_CN");

    std::wstring dest;
    size_t const wcs_len = mbstowcs(NULL, src.c_str(), 0);
    std::vector<wchar_t> tmp(wcs_len + 1);
    mbstowcs(&tmp[0], src.c_str(), src.size());

    dest.assign(tmp.begin(), tmp.end() - 1);

    return dest;
}

} // namespace lzstring