src = ['main.cpp', 'motion.cpp', 'obstacle_map.cpp', 'sensor-gen.cpp']
sim_includes = include_directories('.')

sim_lib = static_library(
    'sim_lib', src,
    link_with : [common_lib, hal_lib],
    include_directories : [common_includes, hal_includes],
    dependencies : stb_dep
)

executable(
    'sim', src,
    link_with : [common_lib, hal_lib],
    include_directories : [common_includes, hal_includes],
    dependencies : stb_dep
)
