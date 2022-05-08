navigation_includes = include_directories('.')

subdir('common')
subdir('events')
subdir('robot')
subdir('sim')
subdir('hal')

navigation_lib = static_library(
    'navigation_lib',
    common_src + events_src + robot_src,
    include_directories : navigation_includes,
)

navigation_dep = declare_dependency(
    link_with: navigation_lib,
    include_directories: navigation_includes,
)

executable(
    'robot', 'robot_main.cpp',
    dependencies: [ navigation_dep, hal_impl_dep, thread_dep, grpc_dep ],
)

executable(
    'sim', sim_src + [ 'sim_main.cpp' ],
    dependencies: [ navigation_dep, sim_hal_impl_dep ]
)
