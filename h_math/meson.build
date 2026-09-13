cc_h_math_src_files = files(
    'h_math.c'
)

cc_h_math_dependencies = [
    libm_external_dp
]

cc_h_math_static = static_library('cc_h_math_static',
    cc_h_math_src_files,
    dependencies: cc_h_math_dependencies
)

cc_h_math_static_dep = declare_dependency(
    link_with: cc_h_math_static,
    dependencies: cc_h_math_dependencies,
    include_directories: include_directories('.')
) 