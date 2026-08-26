cmake_minimum_required(VERSION 3.20)

project(app LANGUAGES C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#add_compile_options(-Wall -Wpedantic -Wextra -Werror)

add_subdirectory(application)
add_subdirectory(draw)
add_subdirectory(struct_utils)