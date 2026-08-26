add_library(cc_dyn_list_static STATIC dyn_list.c)

target_include_directories(cc_dyn_list_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
