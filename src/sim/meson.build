sim_includes = include_directories('.')

subdir('sim_hal_impl')

src = [ 'main.cpp', 'motion.cpp', 'obstacle_map.cpp', 'polygon.cpp' ]

sim_lib = static_library(
    'sim_lib', src,
    link_with : [common_lib, sim_hal_impl_lib],
    include_directories : [common_includes, hal_includes, sim_hal_impl_includes],
)

executable(
    'sim', src,
    link_with : [common_lib, sim_hal_impl_lib],
    include_directories : [common_includes, hal_includes, sim_hal_impl_includes],
)
