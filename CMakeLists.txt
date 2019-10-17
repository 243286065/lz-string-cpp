cmake_minimum_required (VERSION 2.8)
project(demo)
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(demo ${DIR_SRCS})