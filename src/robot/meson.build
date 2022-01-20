src = [ 'loop.cpp', 'drivetrain.cpp', 'potential_map.cpp', 'robot.cpp' ]

robot_includes = include_directories('.')

robot_lib = static_library(
    'robot_lib', src,
    link_with : [common_lib],
    include_directories : [common_includes, hal_includes],
)

thread = dependency('threads')

executable(
    'robot', src + [ 'main.cpp' ],
    link_with : [common_lib, hal_impl_lib],
    include_directories : [common_includes, hal_includes, hal_impl_includes],
    dependencies: [asio, thread]
)
