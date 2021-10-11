sim_includes = include_directories('.')

subdir('sim_hal_impl')

src = [ 'obstacle_map.cpp', 'physics_sim.cpp', 'polygon.cpp' ]

sim_lib = static_library(
    'sim_lib', src,
    link_with : [common_lib, sim_hal_impl_lib, robot_lib],
    include_directories : [common_includes, hal_includes, robot_includes, sim_hal_impl_includes],
)

executable(
    'sim', src + [ 'main.cpp' ],
    link_with : [common_lib, sim_hal_impl_lib, robot_lib],
    include_directories : [common_includes, hal_includes, robot_includes, sim_hal_impl_includes],
)
