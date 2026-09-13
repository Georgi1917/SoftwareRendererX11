project('software-renderer', 'c')

compiler_c = meson.get_compiler('c')

libx_external_dp = compiler_c.find_library('X11')
libm_external_dp = compiler_c.find_library('m')

sub_dirs = [
    'h_math',
    'struct_utils',
    'draw',
    'application'
]

foreach  subdir: sub_dirs 
    subdir(subdir)
endforeach
