src = [
    'hal_provider_impl.cpp',
    'TG30_lidar.cpp',
]

hal_impl_includes = include_directories('.')

hal_impl_lib = static_library(
    'hal_impl_lib', src,
    link_with : common_lib,
    include_directories : [common_includes, hal_includes],
    dependencies: asio,
)
