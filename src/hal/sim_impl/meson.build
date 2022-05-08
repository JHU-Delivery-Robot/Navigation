sim_hal_impl_includes = include_directories('.')
sim_hal_impl_src = files([
    'beam_model.cpp',
    'distance_sensor_model.cpp',
    'hal_provider_sim_impl.cpp',
    'infrared_sensor_sim_impl.cpp',
    'lidar_scanner_sim_impl.cpp',
    'motor_sim_impl.cpp',
    'positioning_sim_impl.cpp',
    'ultrasonic_sensor_sim_impl.cpp',
])

sim_hal_impl_lib = static_library(
    'sim_hal_impl_lib', sim_hal_impl_src + common_src + events_src + robot_src,
    include_directories : navigation_includes,
)

sim_hal_impl_dep = declare_dependency(
    link_with: sim_hal_impl_lib,
    include_directories: [ sim_hal_impl_includes ],
)
