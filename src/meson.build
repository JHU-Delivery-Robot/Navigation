navigation_includes = include_directories('.')

subdir('common')
subdir('comms')
subdir('events')
subdir('hal')
subdir('robot')
subdir('sim')

# All navigation robot code common to both simulation and hardware builds
navigation_lib = static_library(
    'navigation_lib',
    common_src + comms_src + events_src + robot_src,
    include_directories : navigation_includes,
    dependencies: [ dl_dep, bcrypt_dep, dbghelp_dep, proto_dep, grpc_dep, json_dep ],
)

navigation_dep = declare_dependency(
    link_with: navigation_lib,
    include_directories: navigation_includes,
    dependencies: [ dl_dep, bcrypt_dep, dbghelp_dep, proto_dep, grpc_dep, json_dep ],
)

# Simulation-only code
sim_lib = static_library(
    'sim_lib', sim_hal_impl_src + sim_src,
    include_directories : navigation_includes,
    dependencies: [ navigation_dep, json_dep ],
)

sim_dep = declare_dependency(
    link_with: sim_lib,
    include_directories: [ sim_hal_impl_includes ],
    dependencies: [ navigation_dep, json_dep ],
)

# Actual hardware-only code
hardware_lib = static_library(
    'hardware_lib',
    hal_impl_src,
    include_directories : navigation_includes,
    dependencies: [ asio_dep, thread_dep, navigation_dep ],
)

hardware_dep = declare_dependency(
    link_with: hardware_lib,
    include_directories: [ hal_impl_includes ],
    dependencies: [ asio_dep, thread_dep, navigation_dep ],
)

# Hardware-build
executable(
    'navigation_robot', 'robot_main.cpp',
    dependencies: [ hardware_dep ],
)

# Simulation-build
executable(
    'navigation_sim', 'sim_main.cpp',
    dependencies: [ sim_dep ]
)
