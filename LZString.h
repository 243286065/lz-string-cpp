#ifndef LZW_LZSTRING_H_
#define LZW_LZSTRING_H_

#include <string>
#include <unordered_map>
#include <functional>

namespace lzw
{

class LZString
{
	typedef std::unordered_map<char, char> CharDict;
	typedef std::unordered_map<std::wstring, int> StringDict;
	typedef std::unordered_map<std::wstring, bool> StringFlagDict;
	typedef std::function<wchar_t(int)> GetCharFromIntFunc;
	typedef std::function<wchar_t(int)> GetNextCharFunc;

public:
	LZString();
	~LZString();

	std::string CompressToBase64(const std::wstring &input);
	std::wstring DeCompressFromBase64(const std::string &input);

	std::wstring CompressToUTF16(const std::wstring &input);
	std::wstring DeCompressFromUTF16(const std::wstring &input);

	std::string CompressToEncodedURIComponent(const std::wstring &input);
	std::wstring DeCompressFromEncodedURIComponent(const std::string &input);

	void CompressToUint8Array(const std::wstring &input, std::vector<uint8_t> &res);
	std::wstring DeCompressFromUint8Array(const std::vector<uint8_t> &input);

	std::wstring Compress(const std::wstring &input);
	std::wstring DeCompress(const std::wstring &input);

private:
	void CreateBaseDict(const std::string &alphabet, CharDict &dict);

	std::wstring Compress(const std::wstring &input, const int bitsPerChar, GetCharFromIntFunc func);
	std::wstring DeCompress(const int length, const int resetValue, GetNextCharFunc func);

	CharDict m_keyStrBase64Dict;
	CharDict m_keyStrUrisafeDict;
};

} // namespace lzw

#endif