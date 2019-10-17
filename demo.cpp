#include "LZString.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if(argc == 2) {
        lzw::LZString encoder;
        std::string input(argv[1]);
        std::string compressStr;
        std::string decompressStr;

        std::cout << "str: " << input << std::endl;

        /*std::cout << "[base64]" << std::endl;
        compressStr = encoder.CompressToBase64(input);
        decompressStr = encoder.DeCompressFromBase64(compressStr);
        std::cout << "compress: " << compressStr << std::endl;
        std::cout << "uncompress: " << decompressStr << std::endl;
        std::cout << "origin size: " << input.size() << std::endl;
        std::cout <<  "Compress size: " << compressStr.size() << std::endl;
        std::cout << "rate: " << (double(compressStr.size()))/input.size() << std::endl;

        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "[URI]" << std::endl;
        compressStr = encoder.CompressToEncodedURIComponent(input);
        decompressStr = encoder.DeCompressFromEncodedURIComponent(compressStr);
        std::cout << "compress: " << compressStr << std::endl;
        std::cout << "uncompress: " << decompressStr << std::endl;
        std::cout << "origin size: " << input.size() << std::endl;
        std::cout <<  "Compress size: " << compressStr.size() << std::endl;
        std::cout << "rate: " << (double(compressStr.size()))/input.size() << std::endl;*/


        /*std::cout << "-------------------------------------------" << std::endl;
        std::cout << "[UTF16]" << std::endl;
        compressStr = encoder.CompressToUTF16(input);
        decompressStr = encoder.DeCompressFromUTF16(compressStr);
        std::cout << "compress: " << compressStr << std::endl;
        std::cout << "uncompress: " << decompressStr << std::endl;
        std::cout << "origin size: " << input.size() << std::endl;
        std::cout <<  "Compress size: " << compressStr.size() << std::endl;
        std::cout << "rate: " << (double(compressStr.size()))/input.size() << std::endl;*/

        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "[LZW]" << std::endl;
        compressStr = encoder.Compress(input);
        decompressStr = encoder.DeCompress(compressStr);
        std::cout << "compress: " << compressStr << std::endl;
        std::cout << "uncompress: " << decompressStr << std::endl;
        std::cout << "origin size: " << input.size() << std::endl;
        std::cout <<  "Compress size: " << compressStr.size() << std::endl;
        std::cout << "rate: " << (double(compressStr.size()))/input.size() << std::endl;


        /*std::cout << "-------------------------------------------" << std::endl;
        std::cout << "[LZW uint8 array]" << std::endl;
        std::vector<uint8_t> test;
        encoder.CompressToUint8Array(input, test);
        decompressStr = encoder.DeCompressFromUint8Array(test);
        std::cout << "compress: ";
        for(size_t i = 0;i < test.size(); i++) {
          std::cout << (int)test[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "uncompress: " << decompressStr << std::endl;
        std::cout << "origin size: " << input.size() << std::endl;
        std::cout <<  "Compress size: " << test.size() << std::endl;
        std::cout << "rate: " << (double(test.size()))/input.size() << std::endl;*/

		wchar_t test = 8967;
		std::wcout << test << std::endl;
		system("PAUSE");
    }
} 