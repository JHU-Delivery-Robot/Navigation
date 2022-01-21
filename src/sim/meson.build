sim_includes = include_directories('.')

subdir('sim_hal_impl')

src = [ 'obstacle_map.cpp', 'physics_sim.cpp', 'polygon.cpp' ]

sim_lib = static_library(
    'sim_lib', src + common_src,
    link_with : [ sim_hal_impl_lib, robot_lib ],
    include_directories : [common_includes, hal_includes, robot_includes, sim_hal_impl_includes],
)

executable(
    'sim', src + common_src + [ 'main.cpp', 'config.cpp', 'recording.cpp' ],
    link_with : [ sim_hal_impl_lib, robot_lib ],
    include_directories : [ common_includes, hal_includes, robot_includes, sim_hal_impl_includes ],
)
