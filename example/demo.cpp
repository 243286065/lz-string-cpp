#include "LZString.h"
#include "ConvertStrUtil.h"
#include <iostream>

int main(int argc, char *argv[])
{
  if (argc == 2)
  {
    lzstring::LZString encoder;
    std::wstring input(lzstring::to_wstring(argv[1]));
    std::string compressStr;
    std::wstring compressWStr;
    std::string decompressStr;
    std::wstring decompressWStr;

    std::wcout << "str: " << input << std::endl;

    std::cout << "[base64]" << std::endl;
    compressStr = encoder.CompressToBase64(input);
    decompressWStr = encoder.DeCompressFromBase64(compressStr);
    std::cout << "compress: " << compressStr << std::endl;
    std::wcout << "uncompress: " << decompressWStr << std::endl;
    std::cout << "origin size: " << input.size() << std::endl;
    std::cout << "Compress size: " << compressStr.size() << std::endl;
    std::cout << "rate: " << (double(compressStr.size())) / input.size() << std::endl;

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "[URI]" << std::endl;
    compressStr = encoder.CompressToEncodedURIComponent(input);
    decompressWStr = encoder.DeCompressFromEncodedURIComponent(compressStr);
    std::cout << "compress: " << compressStr << std::endl;
    std::wcout << "uncompress: " << decompressWStr << std::endl;
    std::cout << "origin size: " << input.size() << std::endl;
    std::cout << "Compress size: " << compressStr.size() << std::endl;
    std::cout << "rate: " << (double(compressStr.size())) / input.size() << std::endl;

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "[UTF16]" << std::endl;
    compressWStr = encoder.CompressToUTF16(input);
    decompressWStr = encoder.DeCompressFromUTF16(compressWStr);
    //std::wcout << "compress: " << compressWStr << std::endl;
    std::wcout << "uncompress: " << decompressWStr << std::endl;
    std::cout << "origin size: " << input.size() << std::endl;
    std::cout << "Compress size: " << compressWStr.size() << std::endl;
    std::cout << "rate: " << (double(compressWStr.size())) / input.size() << std::endl;

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "[LZ String]" << std::endl;
    compressWStr = encoder.Compress(input);
    decompressWStr = encoder.DeCompress(compressWStr);
    //std::wcout << "compress: " << compressWStr << std::endl;
    std::wcout << "uncompress: " << decompressWStr << std::endl;
    std::cout << "origin size: " << input.size() << std::endl;
    std::cout << "Compress size: " << compressWStr.size() << std::endl;
    std::cout << "rate: " << (double(compressWStr.size())) / input.size() << std::endl;

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "[LZ String uint8 array]" << std::endl;
    std::vector<uint8_t> test;
    encoder.CompressToUint8Array(input, test);
    decompressWStr = encoder.DeCompressFromUint8Array(test);
    std::cout << "compress: ";
    for (size_t i = 0; i < test.size(); i++)
    {
      std::cout << (int)test[i] << " ";
    }
    std::cout << std::endl;
    std::wcout << "uncompress: " << decompressWStr << std::endl;
    std::cout << "origin size: " << input.size() << std::endl;
    std::cout << "Compress size: " << test.size() << std::endl;
    std::cout << "rate: " << (double(test.size())) / input.size() << std::endl;

    std::cout << "-------------------------------------------" << std::endl;
    std::wstring test34 = L"😘";
    compressStr = encoder.CompressToBase64(test34);
    decompressWStr = encoder.DeCompressFromBase64(compressStr);
    std::cout << compressStr << std::endl;
    std::wcout << decompressWStr << std::endl;
    system("PAUSE");
  }
}