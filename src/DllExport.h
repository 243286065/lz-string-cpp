#ifndef LZ_STRING_CPP_DLL_EXPORT_DEF_H_
#define LZ_STRING_CPP_DLL_EXPORT_DEF_H_

#ifdef _WIN32
#ifdef C_DLL_EXPORT
#define WRAPPER_API __declspec(dllexport)
#else 
#define WRAPPER_API __declspec(dllimport)
#endif
#else
#define WRAPPER_API
#endif // _WIN32

#endif // LZ_STRING_CPP_DLL_EXPORT_DEF_H_