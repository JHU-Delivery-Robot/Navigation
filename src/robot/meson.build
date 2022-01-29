robot_includes = include_directories('.')
robot_src = files([ 'loop.cpp', 'potential_map.cpp', 'robot.cpp', 'speed_controller.cpp' ])

executable(
    'robot', robot_src + common_src + [ 'main.cpp' ],
    include_directories : [ common_includes, hal_includes ],
    dependencies: [ hal_impl_dep, thread_dep ],
)
