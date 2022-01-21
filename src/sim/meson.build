sim_includes = include_directories('.')
sim_src = files([
    'obstacle_map.cpp',
    'physics_sim.cpp', 
    'polygon.cpp',
    'config.cpp',
    'recording.cpp',
])

subdir('sim_hal_impl')

executable(
    'sim', sim_src + common_src + robot_src + [ 'main.cpp' ],
    include_directories : [ common_includes, hal_includes, robot_includes ],
    dependencies: [ sim_hal_impl_dep ]
)
