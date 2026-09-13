add_library(cc_h_math_static STATIC h_math.c)

target_include_directories(cc_h_math_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cc_h_math_static PUBLIC m)
