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
	typedef std::unordered_map<std::string, int> StringDict;
	typedef std::unordered_map<std::string, bool> StringFlagDict;
	typedef std::function<char(int)> GetCharFromIntFunc;
	typedef std::function<char(int)> GetNextCharFunc;

public:
	LZString();
	~LZString();

	std::string CompressToBase64(const std::string& input);
	std::string DeCompressFromBase64(const std::string& input);

	std::string CompressToUTF16(const std::string input);
	std::string DeCompressFromUTF16(const std::string input);

	std::string CompressToEncodedURIComponent(const std::string input);
	std::string DeCompressFromEncodedURIComponent(const std::string input);

	void CompressToUint8Array(const std::string &input, std::vector<uint8_t>& res);
	std::string DeCompressFromUint8Array(const std::vector<uint8_t>& input);

	std::string Compress(const std::string &input);
	std::string DeCompress(const std::string &input);
private:
	void CreateBaseDict(const std::string& alphabet, CharDict& dict);

	std::string Compress(const std::string& input, const int bitsPerChar, GetCharFromIntFunc func);
	std::string DeCompress(const int length, const int resetValue, GetNextCharFunc func);

	CharDict m_keyStrBase64Dict;
	CharDict m_keyStrUrisafeDict;
};

} // namespace lzw

#endif