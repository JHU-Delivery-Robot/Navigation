src = [
    'beam_model.cpp',
    'distance_sensor_model.cpp',
    'hal_provider_sim_impl.cpp',
    'infrared_sensor_sim_impl.cpp',
    'lidar_scanner_sim_impl.cpp',
    'ultrasonic_sensor_sim_impl.cpp',
]

sim_hal_impl_includes = include_directories('.')

sim_hal_impl_lib = static_library(
    'sim_hal_impl_lib', src,
    link_with : common_lib,
    include_directories : [common_includes, hal_includes, sim_includes],
)
