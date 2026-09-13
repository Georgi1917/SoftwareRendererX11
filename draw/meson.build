cc_draw_src = files(
    'draw.c'
)

cc_draw_static_dependencies = [
    cc_h_math_static_dep,
    cc_struct_utils_dep,
    libm_external_dp
]

cc_draw_static = static_library('cc_draw_static',
    cc_draw_src,
    dependencies: cc_draw_static_dependencies
)

cc_draw_static_dep = declare_dependency(
    link_with: cc_draw_static,
    dependencies: cc_draw_static_dependencies,
    include_directories: include_directories('.')
)