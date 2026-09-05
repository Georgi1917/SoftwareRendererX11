cc_struct_utils_src = files(
    'dyn_list.c',
    'screen_buffer.c'
)

cc_struct_utils_static = static_library('cc_struct_utils_static', 
    cc_struct_utils_src
)

cc_struct_utils_dep = declare_dependency(
    link_with: cc_struct_utils_static,
    include_directories: include_directories('.')
)
