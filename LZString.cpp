#include "LZString.h"

#include <math.h>
#include <vector>

namespace lzw
{

const std::string kKeyStrBase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const std::string kKeyStrUriSafe = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-$";

LZString::LZString()
{
	CreateBaseDict(kKeyStrBase64, m_keyStrBase64Dict);
	CreateBaseDict(kKeyStrUriSafe, m_keyStrUrisafeDict);
}

LZString::~LZString() {}

void LZString::CreateBaseDict(const std::string &alphabet, CharDict &dict)
{
	dict.clear();
	for (size_t index = 0; index < alphabet.size(); index++)
	{
		dict[alphabet[index]] = (char)index;
	}
}

std::string LZString::CompressToBase64(const std::string &input)
{
	if (input.empty())
		return std::string();

	std::string res = Compress(input, 6, [](int code) {return kKeyStrBase64[code];});
	switch(res.size() % 4) {
		case 0:
			return res;
		case 1:
			return res+"===";
		case 2:
			return res+"==";
		case 3:
			return res+"=";
		default:
			return "";
	}
}

std::string LZString::DeCompressFromBase64(const std::string &input)
{
	if (input.empty())
		return std::string();

	return DeCompress(input.size(), 32, [this, &input](int index){return m_keyStrBase64Dict[input[index]];});
}

std::string LZString::CompressToUTF16(const std::string input)
{
	if (input.empty())
		return std::string();

	return Compress(input, 15, [](int code) {return (char)(code + 32);});
}

std::string LZString::DeCompressFromUTF16(const std::string input)
{
	if (input.empty())
		return std::string();

	return DeCompress(input.size(), 16384, [&input](int index){return (char)(input[index] - 32);});
}

std::string LZString::CompressToEncodedURIComponent(const std::string input)
{
	if (input.empty())
		return std::string();
	
	return Compress(input, 6, [](int code) {return kKeyStrUriSafe[code];});
}

std::string LZString::DeCompressFromEncodedURIComponent(const std::string input)
{
	if (input.empty())
		return std::string();
	
	return DeCompress(input.size(), 32, [this, &input](int index){return m_keyStrUrisafeDict[input[index]];});
}

void LZString::CompressToUint8Array(const std::string &input, std::vector<uint8_t> &res)
{
	if (input.empty())
		return;

	res.clear();
	std::string compressStr = Compress(input);
	res.assign(compressStr.size()*2, 0);
	for(size_t i = 0; i< compressStr.size(); i++) {
		int currentValue = (int)compressStr[i];
		res[i*2] = (char)(currentValue >> 8);
		res[i*2+1] = (char)(currentValue % 256);
	}
}

std::string LZString::DeCompressFromUint8Array(const std::vector<uint8_t> &input)
{
	if (input.empty())
		return std::string();

	std::string res;
	for(size_t i = 0; i< input.size() /2; i++) {
		res += (char)(input[i*2] *256 + input[i*2+1]);
	}

	return DeCompress(res);
}

std::string LZString::Compress(const std::string &input)
{
	if (input.empty())
		return std::string();

	return Compress(input, 16, [](int code){return (wchar_t)code;});
}

std::string LZString::DeCompress(const std::string &input)
{
	if (input.empty())
		return std::string();

	return DeCompress(input.size(), 32768, [&input](int index) { return input[index];});
}

std::string LZString::Compress(const std::string &input, const int bitsPerChar, GetCharFromIntFunc func)
{
	if (input.empty())
		return std::string();

	int i = 0, value = 0;
	StringDict contextDict;
	StringFlagDict contextFlagDict;
	std::string contextWc = "";
	std::string contextW = "";
	int contextEnlargeIn = 2; // Compensate for the first entry which should not count
	int contextDictSize = 3;
	int contextNumBits = 2;
	std::string contextData;
	int contextDataVal = 0;
	int contextDataPosition = 0;

	for (size_t index = 0; index < input.size(); index++)
	{
		char contextC = input[index];
		std::string contextStr = std::string(1, contextC);
		if (contextDict.find(contextStr) == contextDict.end())
		{
			contextDict[contextStr] = contextDictSize++;
			contextFlagDict[contextStr] = true;
		}

		contextWc = contextW + contextC;
		if (contextDict.find(contextWc) != contextDict.end())
		{
			contextW = contextWc;
		}
		else
		{
			if (contextFlagDict.find(contextW) != contextFlagDict.end())
			{
				if ((int)contextW[0] < 256)
				{
					for (size_t i = 0; i < contextNumBits; i++)
					{
						contextDataVal = (contextDataVal << 1);
						if (contextDataPosition == bitsPerChar - 1)
						{
							contextDataPosition = 0;
							contextData.push_back(func(contextDataVal));
							contextDataVal = 0;
						}
						else
						{
							contextDataPosition++;
						}
					}

					value = contextW[0];
					for (size_t i = 0; i < 8; i++)
					{
						contextDataVal = (contextDataVal << 1) | (value & 1);
						if (contextDataPosition == bitsPerChar - 1)
						{
							contextDataPosition = 0;
							contextData.push_back(func(contextDataVal));
							contextDataVal = 0;
						}
						else
						{
							contextDataPosition++;
						}
						value = value >> 1;
					}
				}
				else
				{
					value = 1;
					for (size_t i = 0; i < contextNumBits; i++)
					{
						contextDataVal = (contextDataVal << 1) | value;
						if (contextDataPosition == bitsPerChar - 1)
						{
							contextDataPosition = 0;
							contextData.push_back(func(contextDataVal));
							contextDataVal = 0;
						}
						else
						{
							contextDataPosition++;
						}
						value = 0;
					}

					value = contextW[0];
					for (size_t i = 0; i < 16; i++)
					{
						contextDataVal = (contextDataVal << 1) | (value & 1);
						if (contextDataPosition == bitsPerChar - 1)
						{
							contextDataPosition = 0;
							contextData.push_back(func(contextDataVal));
							contextDataVal = 0;
						}
						else
						{
							contextDataPosition++;
						}
						value = value >> 1;
					}
				}
				contextEnlargeIn--;
				if (contextEnlargeIn == 0)
				{
					contextEnlargeIn = (int)pow(2, contextNumBits);
					contextNumBits++;
				}
				contextFlagDict.erase(contextW);
			}
			else
			{
				value = contextDict[contextW];
				for (size_t i = 0; i < contextNumBits; i++)
				{
					contextDataVal = (contextDataVal << 1) | (value & 1);
					if (contextDataPosition == bitsPerChar - 1)
					{
						contextDataPosition = 0;
						contextData.push_back(func(contextDataVal));
						contextDataVal = 0;
					}
					else
					{
						contextDataPosition++;
					}
					value = value >> 1;
				}
			}
			contextEnlargeIn--;
			if (contextEnlargeIn == 0)
			{
				contextEnlargeIn = (int)pow(2, contextNumBits);
				contextNumBits++;
			}
			// Add wc to the dictionary
			contextDict[contextWc] = contextDictSize++;
			contextW = contextStr;
		}
	}

	// Output the code for w.
	if (contextW != "")
	{
		if (contextFlagDict.find(contextW) != contextFlagDict.end())
		{
			if (contextW[0] < 256)
			{
				for (size_t i = 0; i < contextNumBits; i++)
				{
					contextDataVal = (contextDataVal << 1);
					if (contextDataPosition == bitsPerChar - 1)
					{
						contextDataPosition = 0;
						contextData.push_back(func(contextDataVal));
						contextDataVal = 0;
					}
					else
					{
						contextDataPosition++;
					}
				}
				value = contextW[0];
				for (size_t i = 0; i < 8; i++)
				{
					contextDataVal = (contextDataVal << 1) | (value & 1);
					if (contextDataPosition == bitsPerChar - 1)
					{
						contextDataPosition = 0;
						contextData.push_back(func(contextDataVal));
						contextDataVal = 0;
					}
					else
					{
						contextDataPosition++;
					}
					value = value >> 1;
				}
			}
			else
			{
				value = 1;
				for (size_t i = 0; i < contextNumBits; i++)
				{
					contextDataVal = (contextDataVal << 1) | value;
					if (contextDataPosition == bitsPerChar - 1)
					{
						contextDataPosition = 0;
						contextData.push_back(func(contextDataVal));
						contextDataVal = 0;
					}
					else
					{
						contextDataPosition++;
					}
					value = 0;
				}
				value = contextW[0];
				for (size_t i = 0; i < 16; i++)
				{
					contextDataVal = (contextDataVal << 1) | (value & 1);
					if (contextDataPosition == bitsPerChar - 1)
					{
						contextDataPosition = 0;
						contextData.push_back(func(contextDataVal));
						contextDataVal = 0;
					}
					else
					{
						contextDataPosition++;
					}
					value = value >> 1;
				}
			}
			contextEnlargeIn--;
			if (contextEnlargeIn == 0)
			{
				contextEnlargeIn = (int)pow(2, contextNumBits);
				contextNumBits++;
			}
			contextFlagDict.erase(contextW);
		}
		else
		{
			value = contextDict[contextW];
			for (size_t i = 0; i < contextNumBits; i++)
			{
				contextDataVal = (contextDataVal << 1) | (value & 1);
				if (contextDataPosition == bitsPerChar - 1)
				{
					contextDataPosition = 0;
					contextData.push_back(func(contextDataVal));
					contextDataVal = 0;
				}
				else
				{
					contextDataPosition++;
				}
				value = value >> 1;
			}
		}

		contextEnlargeIn--;
		if (contextEnlargeIn == 0)
		{
			contextEnlargeIn = (int)pow(2, contextNumBits);
			contextNumBits++;
		}
	}

	// Mark the end of the stream
	value = 2;
	for (size_t i = 0; i < contextNumBits; i++)
	{
		contextDataVal = (contextDataVal << 1) | (value & 1);
		if (contextDataPosition == bitsPerChar - 1)
		{
			contextDataPosition = 0;
			contextData.push_back(func(contextDataVal));
			contextDataVal = 0;
		}
		else
		{
			contextDataPosition++;
		}
		value = value >> 1;
	}

	// Flush the last char
	while (true)
	{
		contextDataVal = (contextDataVal << 1);
		if (contextDataPosition == bitsPerChar - 1)
		{
			int code = func(contextDataVal);
			contextData.push_back(code);
			break;
		}
		else
		{
			contextDataPosition++;
		}
	}

	return contextData;
}

std::string LZString::DeCompress(const int length, const int resetValue, GetNextCharFunc func)
{
	std::vector<std::string> dict;
	int enLargeIn = 4;
	int numBits = 3;
	std::string entry;
	std::string result;
	std::string w;
	int bits = 0, resb, maxpower, power;
	char c = '\0';

	char dataVal = func(0);
	int dataPosition = resetValue;
	int dataIndex = 1;

	for (size_t i = 0; i < 3; i++)
	{
		dict.push_back(std::to_string(i));
	}

	maxpower = (int)pow(2, 2);
	power = 1;
	while (power != maxpower)
	{
		resb = dataVal & dataPosition;
		dataPosition >>= 1;
		if (dataPosition == 0)
		{
			dataPosition = resetValue;
			dataVal = func(dataIndex++);
		}
		bits |= (resb > 0 ? 1 : 0) * power;
		power <<= 1;
	}

	switch (bits)
	{
	case 0:
		bits = 0;
		maxpower = (int)pow(2, 8);
		power = 1;
		while (power != maxpower)
		{
			resb = dataVal & dataPosition;
			dataPosition >>= 1;
			if (dataPosition == 0)
			{
				dataPosition = resetValue;
				dataVal = func(dataIndex++);
			}
			bits |= (resb > 0 ? 1 : 0) * power;
			power <<= 1;
		}
		c = (char)bits;
		break;
	case 1:
		bits = 0;
		maxpower = (int)pow(2, 16);
		power = 1;
		while (power != maxpower)
		{
			resb = dataVal & dataPosition;
			dataPosition >>= 1;
			if (dataPosition == 0)
			{
				dataPosition = resetValue;
				dataVal = func(dataIndex++);
			}
			bits |= (resb > 0 ? 1 : 0) * power;
			power <<= 1;
		}
		c = (char)bits;
		break;
	case 2:
		return "";
	}
	w = std::string(1, c);
	dict.push_back(w);
	result.push_back(c);
	while (true)
	{
		if (dataIndex > length)
		{
			return "";
		}

		bits = 0;
		maxpower = (int)pow(2, numBits);
		power = 1;
		while (power != maxpower)
		{
			resb = dataVal & dataPosition;
			dataPosition >>= 1;
			if (dataPosition == 0)
			{
				dataPosition = resetValue;
				dataVal = func(dataIndex++);
			}
			bits |= (resb > 0 ? 1 : 0) * power;
			power <<= 1;
		}

		int c2;
		switch (c2 = bits)
		{
		case (char)0:
			bits = 0;
			maxpower = (int)pow(2, 8);
			power = 1;
			while (power != maxpower)
			{
				resb = dataVal & dataPosition;
				dataPosition >>= 1;
				if (dataPosition == 0)
				{
					dataPosition = resetValue;
					dataVal = func(dataIndex++);
				}
				bits |= (resb > 0 ? 1 : 0) * power;
				power <<= 1;
			}

			c2 = dict.size();
			dict.push_back(std::string(1, (char)bits));
			enLargeIn--;
			break;
		case (char)1:
			bits = 0;
			maxpower = (int)pow(2, 16);
			power = 1;
			while (power != maxpower)
			{
				resb = dataVal & dataPosition;
				dataPosition >>= 1;
				if (dataPosition == 0)
				{
					dataPosition = resetValue;
					dataVal = func(dataIndex++);
				}
				bits |= (resb > 0 ? 1 : 0) * power;
				power <<= 1;
			}

			c2 = dict.size();
			dict.push_back(std::string(1, (char)bits));
			enLargeIn--;
			break;
		case (char)2:
			return result;
		}

		if (enLargeIn == 0)
		{
			enLargeIn = (int)pow(2, numBits);
			numBits++;
		}

		if (dict.size() - 1 >= c2)
		{
			entry = dict[c2];
		}
		else
		{
			if (c2 == dict.size())
			{
				entry += w + w[0];
			}
			else
			{
				return std::string();
			}
		}

		result += entry;

		// Add w+entry[0] to the dictionary
		dict.push_back(w + entry[0]);
		enLargeIn--;

		w = entry;

		if (enLargeIn == 0)
		{
			enLargeIn = (int)pow(2, numBits);
			numBits++;
		}
	}
}

} // namespace lzw