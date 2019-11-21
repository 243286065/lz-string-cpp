from conans import ConanFile, CMake, tools


class LzstringcppConan(ConanFile):
    name = "LZStringcpp"
    version = "1.1.0"
    license = "MIT"
    author = "xl"
    url = "https://github.com/243286065/lz-string-cpp/tree/dev"
    description = "C++ Class implementation of lz-string (based on https://github.com/pieroxy/lz-string)"
    topics = ("LZS", "Compress")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        self.run("git clone -b dev https://github.com/243286065/lz-string-cpp.git")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
        tools.replace_in_file("lz-string-cpp/CMakeLists.txt", "project(LZStringcpp)",
                              '''project(LZStringcpp)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="lz-string-cpp")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/lz-string-cpp %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("LZString.h", dst="include/LZString", src="lz-string-cpp/src")
        self.copy("DllExport.h", dst="include/LZString", src="lz-string-cpp/src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["LZStringcpp"]

