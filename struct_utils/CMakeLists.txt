add_library(cc_struct_utils_static STATIC dyn_list.c screen_buffer.c)

target_include_directories(cc_struct_utils_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
